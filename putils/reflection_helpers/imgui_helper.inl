#include "imgui_helper.hpp"

// stl
#include <concepts>
#include <map>
#include <memory>

// imgui
#include <imgui.h>

// magic_enum
#include <magic_enum.hpp>

// meta
#include "putils/meta/concepts/callable.hpp"
#include "putils/meta/concepts/specialization.hpp"
#include "putils/meta/concepts/streamable.hpp"
#include "putils/meta/fwd.hpp"
#include "putils/meta/traits/function_return_type.hpp"
#include "putils/meta/traits/function_arguments.hpp"

// reflection
#include "putils/reflection.hpp"

// putils
#include "putils/string.hpp"
#include "putils/color.hpp"
#include "putils/lengthof.hpp"
#include "putils/to_string.hpp"
#include "putils/profiling.hpp"

namespace putils::reflection {
	template<typename E>
	bool imgui_enum_combo(const char * label, E & e) noexcept {
		PUTILS_PROFILING_SCOPE;

		using underyling = std::underlying_type_t<E>;
		static_assert(std::is_same_v<underyling, int> || std::is_same_v<underyling, unsigned int>);

		static putils::string<64> names[magic_enum::enum_count<E>()];
		static bool first = true;
		if (first) {
			for (int i = 0; i < lengthof(names); ++i)
				names[i] = magic_enum::enum_names<E>()[i];
			first = false;
		}
		return ImGui::Combo(
			label, (int *)&e,
			[](void *, int idx, const char ** out) { *out = names[idx].c_str(); return true; },
			nullptr, (int)lengthof(names)
		);
	}

	namespace detail::imgui {
		template<std::invocable F>
		void display_in_columns(const char * name, F && f) noexcept {
			PUTILS_PROFILING_SCOPE;

			if (name == nullptr) {
				f();
				return;
			}

			ImGui::Columns(2);
			ImGui::Text("%s", name);
			ImGui::NextColumn();
			f();
			ImGui::Columns();
		}

		template<typename Member>
		putils::string<64> get_id(const char * name, Member && member) noexcept {
			PUTILS_PROFILING_SCOPE;

			if (name == nullptr)
				return putils::string<64>("##") + (intptr_t)&member;
			return putils::string<64>("##%s", name) + (intptr_t)&member;
		}

		template<typename Member>
		putils::string<64> get_name_with_id(const char * name, Member && member) noexcept {
			PUTILS_PROFILING_SCOPE;

			if (name == nullptr)
				return putils::string<64>("##") + (intptr_t)&member;
			return putils::string<64>("%s##", name) + (intptr_t)&member;
		}
	}

	template<typename TRef>
	void imgui_edit(TRef && obj) noexcept {
		PUTILS_PROFILING_SCOPE;

		using T = std::decay_t<TRef>;
		if constexpr (putils::reflection::is_reflectible<T>())
			putils::reflection::for_each_attribute(obj, [](const auto & attr) {
				imgui_edit(attr.name, attr.member);
			});
		else
			imgui_edit(nullptr, FWD(obj));
	}

	template<typename TRef>
	void imgui_edit(const char * name, TRef && obj) noexcept {
		PUTILS_PROFILING_SCOPE;

		using TNoRef = std::remove_reference_t<TRef>;
		constexpr bool is_const = std::is_const<TNoRef>();

		using T = std::decay_t<TRef>;
		const auto id = detail::imgui::get_id(name, obj);
		const auto name_with_id = detail::imgui::get_name_with_id(name, obj);

		if constexpr (!putils::callable<T> && (std::is_pointer<TNoRef>() || putils::specialization<T, std::unique_ptr> || putils::specialization<T, std::shared_ptr> || putils::specialization<T, std::optional>)) {
			if (obj) {
				if constexpr (std::is_void<std::remove_pointer_t<TNoRef>>())
					ImGui::Text("%p", obj);
				else
					imgui_edit(name, FWD(*obj));
			}
			else
				detail::imgui::display_in_columns(name, []() noexcept {
					ImGui::Text("null");
				});
		}

		else if constexpr (requires { obj.c_str(); }) {
			detail::imgui::display_in_columns(name, [&]() noexcept {
				if constexpr (is_const) {
					ImGui::Text("%s", obj.c_str());
				}
				else {
					putils::string<1024> s = obj.c_str();
					ImGui::PushItemWidth(-1.f);
					if (ImGui::InputText(id.c_str(), s.begin(), s.max_size, ImGuiInputTextFlags_EnterReturnsTrue))
						obj = s.c_str();
					ImGui::PopItemWidth();
				}
			});
		}

		// else if constexpr (std::is_same_v<T, const char *>::value)
		// 	ImGui::LabelText(name, obj);

		else if constexpr (putils::specialization<T, std::map> || putils::specialization<T, std::unordered_map>) {
			if (ImGui::TreeNode(name_with_id.c_str())) {
				using key_type = typename T::key_type;

				auto to_erase = obj.end();
				int i = 0;
				for (auto it = obj.begin(); it != obj.end(); ++it) {
					auto && [key, value] = *it;

					putils::string<64> tree_node_title("%d", i++);
					constexpr bool key_as_tree_node_title = putils::streamable<putils_typeof(key), std::stringstream>;
					if constexpr (key_as_tree_node_title)
						tree_node_title = putils::to_string(key);

					const auto tree_node_open = ImGui::TreeNode(tree_node_title.c_str());
					if constexpr (!is_const)
						if (ImGui::BeginPopupContextItem()) {
							if (ImGui::Button("Remove"))
								to_erase = it;
							ImGui::EndPopup();
						}

					if (tree_node_open) {
						if constexpr (key_as_tree_node_title)
							imgui_edit(FWD(value));
						else {
							if (ImGui::TreeNode(detail::imgui::get_name_with_id("key", key).c_str())) {
								imgui_edit(key);
								ImGui::TreePop();
							}

							if (ImGui::TreeNode(detail::imgui::get_name_with_id("value", value).c_str())) {
								imgui_edit(FWD(value));
								ImGui::TreePop();
							}
						}
						ImGui::TreePop();
					}
				}

				if constexpr (!is_const) {
					if (to_erase != obj.end())
						obj.erase(to_erase);

					ImGui::Columns(3);
					using key_type = typename T::key_type;
					static key_type key;
					imgui_edit(key);

					ImGui::NextColumn();
					using mapped_type = typename T::mapped_type;
					static mapped_type value;
					imgui_edit(value);

					ImGui::NextColumn();

					if (const auto it = obj.find(key); it != obj.end())
						ImGui::TextDisabled("Key already in map");
					else {
						if (ImGui::Button("Add")) {
							obj.emplace(key, value);
							key = key_type{};
							value = mapped_type{};
						}
					}

					ImGui::Columns();
				}

				ImGui::TreePop();
			}
		}

		else if constexpr (std::ranges::range<TNoRef>) {
			if (ImGui::TreeNode(name_with_id.c_str())) {
				int i = 0;

				auto to_erase = std::end(obj);
				for (auto it = std::begin(obj); it != std::end(obj); ++it) {
					const auto tree_node_open = ImGui::TreeNode(putils::string<64>("%d", i++).c_str());
					if constexpr (!is_const)
						if (ImGui::BeginPopupContextItem()) {
							if (ImGui::Button("Remove"))
								to_erase = it;
							ImGui::EndPopup();
						}

					if (tree_node_open) {
						imgui_edit(FWD(*it));
						ImGui::TreePop();
					}
				}

				if constexpr (!is_const) {
					if constexpr (requires { obj.erase(to_erase); })
						if (to_erase != std::end(obj))
							obj.erase(to_erase);

					if constexpr (requires { obj.emplace_back(); })
						if (ImGui::MenuItem("Add"))
							obj.emplace_back();
				}

				ImGui::TreePop();
			}
		}

		else if constexpr (std::is_same_v<T, putils::color>) {
			detail::imgui::display_in_columns(name, [&]() noexcept {
				auto normalized = putils::to_normalized_color(obj);
				const ImVec4 col = { normalized.r, normalized.g, normalized.b, normalized.a };
				const auto should_open_popup = ImGui::ColorButton(id.c_str(), col);

				if constexpr (!is_const) {
					if (should_open_popup)
						ImGui::OpenPopup(name_with_id.c_str());

					if (ImGui::BeginPopup(name_with_id.c_str())) {
						if (ImGui::ColorPicker4(name_with_id.c_str(), normalized.attributes))
							obj = putils::to_color(normalized);
						ImGui::EndPopup();
					}
				}
			});
		}

		else if constexpr (std::is_same_v<T, putils::normalized_color>) {
			detail::imgui::display_in_columns(name, [&]() noexcept {
				const ImVec4 col = { obj.r, obj.g, obj.b, obj.a };

				const auto should_open_popup = ImGui::ColorButton(id.c_str(), col);
				if constexpr (!is_const) {
					if (should_open_popup)
						ImGui::OpenPopup(name_with_id.c_str());

					if (ImGui::BeginPopup(name_with_id.c_str())) {
						ImGui::ColorPicker4(name_with_id.c_str(), obj.attributes);
						ImGui::EndPopup();
					}
				}
			});
		}

		else if constexpr (putils::callable<T>) {
			if (obj == nullptr)
				return;

			using args = putils::function_arguments<T>;

			if constexpr (std::is_default_constructible<args>()) {
				if (ImGui::TreeNode(name_with_id.c_str())) {
					static args args;

					using return_type = putils::function_return_type<T>;
					if constexpr (!std::is_same<return_type, void>() && std::is_default_constructible<return_type>()) {
						static return_type ret;
						if (ImGui::Button("Call"))
							ret = std::apply(obj, args);
						imgui_edit("Return value", (const return_type &)ret);
					}
					else {
						if (ImGui::Button("Call"))
							std::apply(obj, args);
					}

					size_t i = 0;
					putils::tuple_for_each(args, [&](auto & arg) {
						imgui_edit(putils::string<64>("Arg %zu", i).c_str(), arg);
						++i;
					});

					ImGui::TreePop();
				}
			}
		}

		else if constexpr (putils::reflection::is_reflectible<T>()) {
			if (ImGui::TreeNode(name_with_id.c_str())) {
				putils::reflection::for_each_attribute(FWD(obj), [](const auto & attr) noexcept {
					imgui_edit(attr.name, FWD(attr.member));
				});
				ImGui::TreePop();
			}
		}

		else if constexpr (std::is_enum<T>()) {
			if constexpr (magic_enum::enum_count<T>() > 0) {
				detail::imgui::display_in_columns(name, [&]() noexcept {
					if constexpr (is_const)
						ImGui::Text("%s", magic_enum::enum_name(obj).data());
					else
						imgui_enum_combo(id.c_str(), obj);
				});
			}
			else {
				if constexpr (is_const)
					imgui_edit(name, std::underlying_type_t<T>(obj));
				else {
					auto non_enum_value = std::underlying_type_t<T>(obj);
					imgui_edit(name, non_enum_value);
					obj = T(non_enum_value);
				}
			}
		}

		else if constexpr (std::is_same_v<T, bool>) {
			detail::imgui::display_in_columns(name, [&]() noexcept {
				if constexpr (is_const)
					ImGui::Text(obj ? "true" : "false");
				else
					ImGui::Checkbox(id.c_str(), &obj);
			});
		}

		else if constexpr (std::is_scalar_v<T> || std::is_same_v<T, std::ptrdiff_t>) {
			if constexpr (is_const) {
				detail::imgui::display_in_columns(name, [&] {
					if constexpr (std::is_same_v<T, short>)
						ImGui::Text("%hd", obj);
					else if constexpr (std::is_same_v<T, unsigned short>)
						ImGui::Text("%hu", obj);
					else if constexpr (std::is_same_v<T, int>)
						ImGui::Text("%d", obj);
					else if constexpr (std::is_same_v<T, unsigned int>)
						ImGui::Text("%u", obj);
					else if constexpr (std::is_same_v<T, long>)
						ImGui::Text("%ld", obj);
					else if constexpr (std::is_same_v<T, unsigned long>)
						ImGui::Text("%lu", obj);
					else if constexpr (std::is_same_v<T, size_t>)
						ImGui::Text("%zu", obj);
					else if constexpr (std::is_same_v<T, float>)
						ImGui::Text("%f", obj);
					else if constexpr (std::is_same_v<T, double>)
						ImGui::Text("%d", obj);
					else
						ImGui::Text("<unknown type>");
				});
			}
			else {
				detail::imgui::display_in_columns(name, [&] {
					ImGui::PushItemWidth(-1.f);
					constexpr auto data_type = [] {
						if constexpr (std::is_integral_v<T>) {
							if constexpr (std::is_signed_v<T>) {
								switch (sizeof(T)) {
									case 1: return ImGuiDataType_S8;
									case 2: return ImGuiDataType_S16;
									case 4: return ImGuiDataType_S32;
									case 8: return ImGuiDataType_S64;
									default: assert(false);
								}
							}
							else {
								switch (sizeof(T)) {
									case 1: return ImGuiDataType_U8;
									case 2: return ImGuiDataType_U16;
									case 4: return ImGuiDataType_U32;
									case 8: return ImGuiDataType_U64;
									default: assert(false);
								}
							}
						}
						else if constexpr (std::is_same_v<T, double>)
							return ImGuiDataType_Double;
						else if constexpr (std::is_same_v<T, float>)
							return ImGuiDataType_Float;
						else
							assert(false);
					}();
					auto val = obj;
					if (ImGui::InputScalar(id.c_str(), data_type, &val, nullptr, nullptr, nullptr, ImGuiInputTextFlags_EnterReturnsTrue))
						obj = val;
					ImGui::PopItemWidth();
				});
			}
		}

		else {
			detail::imgui::display_in_columns(name, [] {
				ImGui::Text("unknown type");
			});
		}
	}
}