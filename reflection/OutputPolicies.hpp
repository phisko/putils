#pragma once

#include <cctype>
#include <sstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <type_traits>

#include "meta/type.hpp"
#include "to_string.hpp"
#include "chop.hpp"
#include "json.hpp"
#include "meta/for_each.hpp"
#include "traits.hpp"

namespace putils {
    namespace OutputPolicies {
        struct Json {
            static void startSerialize(std::ostream & s) { s << '{'; }
            static void endSerialize(std::ostream & s) { s << '}'; }
            static void serializeNewField(std::ostream & s) { s << ", "; }

            template<typename T, typename Tuple>
            static void unserialize(std::istream & s, T & obj, const Tuple & tuple) {
                std::size_t openBraces = 0;
                std::string str;

                while (std::isspace(s.peek()))
                    s.get();

                while (s) {
                    const char c = s.get();
                    if (c == '{' || c == '[')
                        ++openBraces;
                    else if (c == '}' || c == ']')
                        --openBraces;

                    str.append(1, c);

                    if (!openBraces)
                        break;
                }

                const auto jsonObject = putils::json::parse(str);

                putils::tuple_for_each(tuple, [&obj, &jsonObject](const auto & attr) {
					using MemberType = std::remove_reference_t<decltype(std::declval<T>().*(attr.second))>;

					if constexpr (!std::is_const<MemberType>::value && !std::is_abstract<MemberType>::value) {
						const auto it = jsonObject.find(std::string(attr.first));
						if (it == jsonObject.end())
							return;

						unserialize(*it, obj.*(attr.second));
					}
                });
            };

            /*
             * Bools
             */

            static void serialize(std::ostream & s, std::string_view name, bool value) {
                s << '"' << name << '"' << ": " << std::boolalpha << value << std::noboolalpha;
            }

            static void unserialize(const nlohmann::json & object, bool & attr) {
				attr = object;
            }

            /*
             * Maps
             */

            template<typename Map>
            static void printMap(std::ostream & s, std::string_view name, Map && map) {
                s << '"' << name << '"' << ": {";
                bool first = true;
                for (const auto & pair : map) {
                    if (!first)
                        s << ", ";

                    serialize(s, putils::toString(pair.first), pair.second);
                    first = false;
                }
                s << "}";
            }

            template<typename Key, typename Value, typename Map>
            static void unserializeMap(const nlohmann::json & object, Map & map) {
                map.clear();

				for (auto it = object.begin(); it != object.end(); ++it) {
                    Key k;
					unserialize(it.key(), k);
					unserialize(it.value(), map[k]);
                }
            }

            template<typename Key, typename Value>
            static void serialize(std::ostream & s, std::string_view name,
                                  const std::unordered_map<Key, Value> & map) { printMap(s, name, map); }
            template<typename Key, typename Value>
            static void unserialize(const nlohmann::json & object, std::unordered_map<Key, Value> & map) { unserializeMap<Key, Value>(object, map); }


            template<typename Key, typename Value>
            static void serialize(std::ostream & s, std::string_view name,
                                  const std::map<Key, Value> & map) { printMap(s, name, map); }
            template<typename Key, typename Value>
            static void unserialize(const nlohmann::json & object, std::map<Key, Value> & map) { unserializeMap<Key, Value>(object, map); }

            /*
             * Ptrs
             */

            template<typename Ptr>
            static void printPtr(std::ostream & s, std::string_view name, Ptr && ptr) { serialize(s, name, *ptr); }

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const std::unique_ptr<T> & ptr) { printPtr(s, name, ptr); }
            template<typename T>
            static void unserialize(const nlohmann::json &, std::unique_ptr<T> & ptr) {
				ptr = nullptr;
            }

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const std::shared_ptr<T> & ptr) { printPtr(s, name, ptr); }
            template<typename T>
            static void unserialize(const nlohmann::json &, std::shared_ptr<T> & ptr) {
				ptr = nullptr;
            }

            /*
             * Containers
             */

            template<typename Container>
            static void printContainer(std::ostream & s, std::string_view name, const Container & container) {
                s << '"' << name << '"' << ": [";
                bool first = true;
                for (const auto & val : container) {
					if constexpr (putils::is_streamable<std::ostream, putils_typeof(val)>::value) {
						if (!first)
							s << ",";

						if constexpr (std::is_constructible<std::string, decltype(val)>::value || std::is_pointer<putils_typeof(val)>::value)
							s << '"' << val << '"';
						else
							s << val;

						first = false;
					}
                }

                s << "]";
            }

            template<template<typename, typename> typename Container, typename T, typename _>
            static void unserializeContainer(const nlohmann::json & object, Container<T, _> & attr) {
                attr.clear();

				if constexpr (!std::is_pointer<T>::value)
					return;

				for (const auto & element : object) {
                    T obj;
                    unserializeContainerElement(element, obj);
					attr.emplace_back(std::move(obj));
				}
            }

			template<typename T>
			static void unserializeContainerElement(const nlohmann::json & object, T & attr) {
				unserialize(object, attr);
            }

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const std::list<T> & container) { printContainer(s, name, container); }
            template<typename T>
            static void unserialize(const nlohmann::json & object, std::list<T> & attr) { unserializeContainer(object, attr); }

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const std::vector<T> & container) { printContainer(s, name, container); }
            template<typename T>
            static void unserialize(const nlohmann::json & object, std::vector<T> & attr) { unserializeContainer(object, attr); }

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const std::set<T> & container) { printContainer(s, name, container); }
            template<typename T>
            static void unserialize(const nlohmann::json & object, std::set<T> & attr) { unserializeContainer(object, attr); }

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const std::unordered_set<T> & container) { printContainer(s, name, container); }
            template<typename T>
            static void unserialize(const nlohmann::json & object, std::unordered_set<T> & attr) { unserializeContainer(object, attr); }

            /*
             * "Normal" objects
             */

            template<typename T>
            static void serialize(std::ostream & s, std::string_view name, const T & attr) {
				if constexpr (std::is_enum<T>::value)
					s << '"' << name << '"' << ": " << (int)attr;
				else if constexpr (std::is_pointer<T>::value)
					printPtr(s, name, attr);
				else if constexpr (std::is_constructible<std::string_view, T>::value)
					s << '"' << name << '"' << ": \"" << attr << "\"";
				else if constexpr (putils::is_streamable<std::ostream, T>::value)
					s << '"' << name << '"' << ": " << attr;
				else
					s << '"' << name << '"' << ": " << '"' << &attr << '"';
            }

			template<typename T>
			static void unserialize(const nlohmann::json & object, T & attr) {
				const auto value = object.dump();

				if constexpr (putils::is_unstreamable<std::stringstream, T>::value) {
					if (value[0] == '"')
						putils::parse(attr, putils::chop(value.substr(1, value.size() - 2)).c_str());
					else
						putils::parse(attr, putils::chop(value).c_str());
				}
			}

            template<typename T>
            static void unserialize(std::istream & s, T & attr) {
				const auto object =  putils::json::parse(s);

				if constexpr (std::is_pointer<T>::value)
					attr = nullptr;
				else if constexpr (std::is_enum<T>::value)
					unserialize(object, (int &)attr);
				else
					unserialize(object, attr);
			}
		};


        struct Default {
            static void startSerialize(std::ostream & s) {}
            static void endSerialize(std::ostream & s) {}
            static void serializeNewField(std::ostream & s) {}

            template<typename T, typename Tuple>
            static void unserialize(std::istream & s, T & obj, const Tuple & tuple) {
                putils::tuple_for_each(tuple, [&s, &obj](const auto & attr) {
                    try {
                        unserialize(s, obj.*(attr.second));
                    }
                    catch (const std::exception & e) {
                        std::cerr << e.what() << std::endl;
                    }
                });
            };

            template<typename T>
            static void serialize(std::ostream &, std::string_view, const std::unique_ptr<T> &) {}

            template<typename T>
            static void unserialize(std::istream &, std::unique_ptr<T> &) {}

            template<typename T>
            static void serialize(std::ostream &, std::string_view, const std::shared_ptr<T> &) {}

            template<typename T>
            static void unserialize(std::istream &, std::shared_ptr<T> &) {}

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value>>
            static void serialize(std::ostream & s, std::string_view name, const T & attr) { s << attr << " "; }

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value>>
            static void unserialize(std::istream & s, T & attr) { s >> attr; }

            template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
            static void serialize(std::ostream & s, std::string_view name, T attr) { s << (int) attr << " "; };

            template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
            static void unserialize(std::istream & s, const T & attr) { s >> (int &) attr; }
        };

        struct Binary {
            static void startSerialize(std::ostream & s) {}
            static void endSerialize(std::ostream & s) {}
            static void serializeNewField(std::ostream & s) {}

            template<typename T, typename Tuple>
            static void unserialize(std::istream & s, T & obj, const Tuple & tuple) {
                putils::tuple_for_each(tuple, [&s, &obj](const auto & attr) {
                    try {
                        unserialize(s, obj.*(attr.second));
                    }
                    catch (const std::exception & e) {
                        std::cerr << e.what() << std::endl;
                    }
                });
            };

            template<typename T>
            static void serialize(std::ostream &, std::string_view, const std::unique_ptr<T> &) {}

            template<typename T>
            static void unserialize(std::istream &, std::unique_ptr<T> &) {}

            template<typename T>
            static void serialize(std::ostream &, std::string_view, const std::shared_ptr<T> &) {}

            template<typename T>
            static void unserialize(std::istream &, std::shared_ptr<T> &) {}

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value>>
            static void serialize(std::ostream & s, std::string_view name, const T & attr) { s.write(&attr, sizeof(attr)); }

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value>>
            static void unserialize(std::istream & s, T & attr) { s.read(&attr, sizeof(attr)); }
        };

        struct None {
            static void startSerialize(std::ostream & s) {}
            static void endSerialize(std::ostream & s) {}
            static void serializeNewField(std::ostream & s) {}
            template<typename T, typename Tuple>
            static void unserialize(std::istream & s, T & obj, const Tuple & tuple) {}
            template<typename Auto>
            static void serialize(std::ostream &, std::string_view, const Auto &) {}
            template<typename Auto>
            static void unserialize(std::istream &, Auto &) {}
        };
    }
}
