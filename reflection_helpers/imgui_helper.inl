#include "imgui_helper.hpp"

#include "imgui.h"
#include "reflection.hpp"
#include "Color.hpp"
#include "magic_enum.hpp"
#include "lengthof.hpp"
#include "universal_functor.hpp"
#include "meta/traits/is_iterable.hpp"
#include "meta/traits/is_function.hpp"
#include "meta/traits/is_specialization.hpp"
#include "meta/traits/function_return_type.hpp"
#include "meta/traits/function_arguments.hpp"

namespace putils::reflection {
	template<typename E>
	bool imguiEnumCombo(const char * label, E & e) noexcept {
		static_assert(std::is_same_v<std::underlying_type_t<E>, int>);

		static putils::string<64> names[putils::magic_enum::enum_count<E>()];
		static bool first = true;
		if (first) {
			for (int i = 0; i < lengthof(names); ++i)
				names[i] = putils::magic_enum::enum_names<E>()[i];
			first = false;
		}
		return ImGui::Combo(label, (int *)&e, [](void *, int idx, const char ** out) { *out = names[idx].c_str(); return true; }, nullptr, (int)lengthof(names));
	}

	namespace detail::imgui {
		putils_member_detector(c_str);
		putils_member_detector(emplace_back);

		template<typename F>
		void displayInColumns(const char * name, F && f) noexcept {
			if (name == nullptr) {
				f();
				return;
			}

			ImGui::Columns(2);
			ImGui::Text(name);
			ImGui::NextColumn();
			f();
			ImGui::Columns();
		}

		template<typename Member>
		putils::string<64> getID(const char * name, Member && member) noexcept {
			if (name == nullptr)
				return putils::string<64>("##") + (intptr_t)&member;
			return putils::string<64>("##%s", name) + (intptr_t)&member;
		}

		template<typename Member>
		putils::string<64> getNameWithID(const char * name, Member && member) noexcept {
			if (name == nullptr)
				return putils::string<64>("##") + (intptr_t)&member;
			return putils::string<64>("%s##", name) + (intptr_t)&member;
		}
	}

	template<typename TRef>
	void imguiEdit(TRef && obj) noexcept {
		using T = std::decay_t<TRef>;
		if constexpr (putils::reflection::has_attributes<T>())
			putils::reflection::for_each_attribute(obj, UNIVERSAL_FUNCTOR(imguiEdit));
		else
			imguiEdit(nullptr, FWD(obj));
	}

	template<typename TRef>
	void imguiEdit(const char * name, TRef && obj) noexcept {
		using TNoRef = std::remove_reference_t<TRef>;
		constexpr bool isConst = std::is_const<TNoRef>();

		using T = std::decay_t<TRef>;
		const auto id = detail::imgui::getID(name, obj);
		const auto nameWithID = detail::imgui::getNameWithID(name, obj);
		
		if constexpr (putils::reflection::has_attributes<T>()) {
			if (ImGui::TreeNode(nameWithID)) {
				putils::reflection::for_each_attribute(obj, UNIVERSAL_FUNCTOR(imguiEdit));
				ImGui::TreePop();
			}
		}

		else if constexpr (std::is_pointer<T>() || putils::is_specialization<T, std::unique_ptr>() || putils::is_specialization<T, std::shared_ptr>() || putils::is_specialization<T, std::optional>()) {
			if (obj) {
				if constexpr (std::is_same<std::remove_pointer_t<T>, void>())
					ImGui::Text("%p", obj);
				else
					imguiEdit(name, *obj);
			}
			else
				detail::imgui::displayInColumns(name, []() noexcept {
					ImGui::Text("null");
				});
		}
	
		else if constexpr (detail::imgui::has_member_c_str<T>()) {
			detail::imgui::displayInColumns(name, [&]() noexcept {
				if constexpr (isConst) {
					ImGui::Text(obj.c_str());
				}
				else {
					putils::string<1024> s = obj.c_str();
					ImGui::PushItemWidth(-1.f);
					if (ImGui::InputText(id, s.begin(), s.max_size, ImGuiInputTextFlags_EnterReturnsTrue))
						obj = s.c_str();
					ImGui::PopItemWidth();
				}
			});
		}
		
		// else if constexpr (std::is_same_v<T, const char *>::value)
		// 	ImGui::LabelText(name, obj);
		
		else if constexpr (putils::is_iterable<T>()) {
			if (ImGui::TreeNode(nameWithID)) {
				int i = 0;
				for (auto&& val : obj) // TODO: check this works with const
					imguiEdit(putils::string<64>("%d", i++), val);
				
				if constexpr (!isConst && detail::imgui::has_member_emplace_back<T>()) {
					if (ImGui::MenuItem("Add"))
						obj.emplace_back();
					static int removeIndex = 0;
					ImGui::Columns(2);
					if (ImGui::MenuItem("Remove")) {
						int i = 0;
						for (auto it = obj.begin(); it != obj.end(); ++it) {
							if (i == removeIndex) {
								obj.erase(it);
								break;
							}
							++i;
						}
					}
					ImGui::NextColumn();
					ImGui::InputInt("##index", &removeIndex);
					ImGui::Columns();
				}

				ImGui::TreePop();
			}
		}

		else if constexpr (std::is_same_v<T, putils::Color>) {
			detail::imgui::displayInColumns(name, [&]() noexcept {
				auto normalized = putils::toNormalizedColor(obj);
				const ImVec4 col = { normalized.r, normalized.g, normalized.b, normalized.a };
				const auto shouldOpenPopup = ImGui::ColorButton(id, col);

				if constexpr (!isConst) {
					if (shouldOpenPopup)
						ImGui::OpenPopup(nameWithID);

					if (ImGui::BeginPopup(nameWithID)) {
						if (ImGui::ColorPicker4(nameWithID, normalized.attributes))
							obj = putils::toColor(normalized);
						ImGui::EndPopup();
					}
				}
			});
		}

		else if constexpr (std::is_same_v<T, putils::NormalizedColor>) {
			detail::imgui::displayInColumns(name, [&]() noexcept {
				const ImVec4 col = {obj.r, obj.g, obj.b, obj.a};

				const auto shouldOpenPopup = ImGui::ColorButton(id, col);
				if constexpr (!isConst) {
					if (shouldOpenPopup)
						ImGui::OpenPopup(nameWithID);

					if (ImGui::BeginPopup(nameWithID)) {
						ImGui::ColorPicker4(nameWithID, obj.attributes);
						ImGui::EndPopup();
					}
				}
			});
		}
	
		else if constexpr (putils::is_function<T>()) {
			if (obj == nullptr)
				return;

			using Args = putils::function_arguments<T>;

			if constexpr (std::is_default_constructible<Args>()) {
				if (ImGui::TreeNode(nameWithID)) {
					static Args args;


					using Ret = putils::function_return_type<T>;
					if constexpr (!std::is_same<Ret, void>() && std::is_default_constructible<Ret>()) {
						static Ret ret;
						if (ImGui::Button("Call"))
							ret = std::apply(obj, args);
						imguiEdit("Return value", (const Ret &)ret);
					}
					else {
						if (ImGui::Button("Call"))
							std::apply(obj, args);
					}

					size_t i = 0;
					putils::tuple_for_each(args, [&](auto & arg) {
						imguiEdit(putils::string<64>("Arg %zu", i), arg);
						++i;
					});

					ImGui::TreePop();
				}
			}
		}

		else if constexpr (std::is_enum<T>()) {
			detail::imgui::displayInColumns(name, [&]() noexcept {
				if constexpr (isConst)
					ImGui::Text(putils::magic_enum::enum_name(obj).data());
				else
					imguiEnumCombo(id, obj);
			});
		}
		
		else if constexpr (std::is_same_v<T, bool>) {
			detail::imgui::displayInColumns(name, [&]() noexcept {
				if constexpr (isConst)
					ImGui::Text(obj ? "true" : "false");
				else
					ImGui::Checkbox(id, &obj);
			});
		}

		else if constexpr (std::is_same_v<T, int>) {
			detail::imgui::displayInColumns(name, [&]() noexcept {
				if constexpr (isConst)
					ImGui::Text("%d", obj);
				else {
					ImGui::PushItemWidth(-1.f);
					auto val = obj;
					if (ImGui::InputInt(id, &val, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
						obj = val;
					ImGui::PopItemWidth();
				}
			});
		}

		else if constexpr (std::is_same_v<T, unsigned int> || std::is_same_v<T, size_t>) {
			detail::imgui::displayInColumns(name, [&]() noexcept {
				if constexpr (isConst)
					ImGui::Text("%zu", obj);
				else {
					ImGui::PushItemWidth(-1.f);
					auto val = obj;
					if (ImGui::InputScalar(id, sizeof(T) == 64 ? ImGuiDataType_U64 : ImGuiDataType_U32, &val, nullptr, nullptr, nullptr, ImGuiInputTextFlags_EnterReturnsTrue))
						obj = val;
					ImGui::PopItemWidth();
				}
			});
		}

		else if constexpr (std::is_same_v<T, float>) {
			detail::imgui::displayInColumns(name, [&]() noexcept {
				if constexpr (isConst)
					ImGui::Text("%f", obj);
				else {
					ImGui::PushItemWidth(-1.f);
					auto val = obj;
					if (ImGui::InputFloat(id, &val, 0.f, 0.f, "%.6f", ImGuiInputTextFlags_EnterReturnsTrue))
						obj = val;
					ImGui::PopItemWidth();
				}
			});
		}
		
		else if constexpr (std::is_same_v<T, double>) {
			detail::imgui::displayInColumns(name, [&]() noexcept {
				if constexpr (isConst)
					ImGui::Text("%d", obj);
				else {
					ImGui::PushItemWidth(-1.f);
					auto val = obj;
					if (ImGui::InputDouble(id, &val, 0.0, 0.0, "%.6f", ImGuiInputTextFlags_EnterReturnsTrue))
						obj = val;
					ImGui::PopItemWidth();
				}
			});
		}
		
		else {
			detail::imgui::displayInColumns(name, [&]() noexcept {
				ImGui::Text("Unknown type");
			});
		}
	}
}