#pragma once

#include "imgui.h"
#include "reflection.hpp"
#include "Color.hpp"
#include "magic_enum.hpp"
#include "lengthof.hpp"

namespace putils::reflection {
	template<typename T>
	void imguiEdit(T & obj);

	template<typename T>
	void imguiDisplay(const T & obj);

	template<typename E>
	void imguiEnumCombo(E & e);

	// Impl

	template<typename E>
	bool imguiEnumCombo(const char * label, E & e) {
		static_assert(std::is_same_v<std::underlying_type_t<E>, int>);

		static putils::string<64> names[putils::magic_enum::enum_count<E>()];
		static bool first = true;
		if (first) {
			for (int i = 0; i < lengthof(names); ++i)
				names[i] = putils::magic_enum::enum_names<E>()[i];
			first = false;
		}
		return ImGui::Combo(label, (int *)&e, [](void *, int idx, const char ** out) { *out = names[idx].c_str(); return true; }, nullptr, lengthof(names));
	}

	namespace detail::imgui {
		putils_member_detector(c_str);
		putils_member_detector(emplace_back);

		template<typename F>
		void displayInColumns(const char * name, F && f) {
			ImGui::Columns(2);
			ImGui::Text(name);
			ImGui::NextColumn();
			f();
			ImGui::Columns();
		}

		template<typename Member>
		putils::string<64> getID(const char * name, Member && member) {
			return putils::string<64>("##%s", name) + (intptr_t)&member;
		}

		template<typename Member>
		putils::string<64> getNameWithID(const char * name, Member && member) {
			return putils::string<64>("%s##", name) + (intptr_t)&member;
		}

		template<typename MemberRef>
		void editAttribute(const char * name, MemberRef && member) {
			using Member = std::remove_reference_t<MemberRef>;
			const auto id = getID(name, member);
			const auto nameWithID = getNameWithID(name, member);

			if constexpr (detail::imgui::has_member_c_str<Member>()) {
				displayInColumns(name, [&] {
					putils::string<1024> s = member.c_str();
					ImGui::PushItemWidth(-1.f);
					if (ImGui::InputText(id, s.begin(), s.max_size, ImGuiInputTextFlags_EnterReturnsTrue))
						member = s.c_str();
					ImGui::PopItemWidth();
				});
			}
			// else if constexpr (std::is_same_v<Member, const char *>::value)
			// 	ImGui::LabelText(name, member);

			else if constexpr (putils::is_iterable<Member>()) {
				if (ImGui::TreeNode(nameWithID)) {
					int i = 0;
					for (auto & val : member)
						editAttribute(putils::string<64>("%d", i++), val);

					if constexpr (detail::imgui::has_member_emplace_back<Member>())
						if (ImGui::MenuItem("Add"))
							member.emplace_back();
					ImGui::TreePop();
				}
			}

			else if constexpr (std::is_same_v<Member, putils::Color>) {
				displayInColumns(name, [&] {
					auto normalized = putils::toNormalizedColor(member);
					const ImVec4 col = { normalized.r, normalized.g, normalized.b, normalized.a };

					if (ImGui::ColorButton(id, col))
						ImGui::OpenPopup(nameWithID);

					if (ImGui::BeginPopup(nameWithID)) {
						if (ImGui::ColorPicker4(nameWithID, normalized.attributes))
							member = putils::toColor(normalized);
						ImGui::EndPopup();
					}
				});
			}
			else if constexpr (std::is_same_v<Member, putils::NormalizedColor>) {
				displayInColumns(name, [&] {
					const ImVec4 col = { member.r, member.g, member.b, member.a };
					if (ImGui::ColorButton(id, col))
						ImGui::OpenPopup(nameWithID);

					if (ImGui::BeginPopup(nameWithID)) {
						ImGui::ColorPicker4(nameWithID, member.attributes);
						ImGui::EndPopup();
					}
				});
			}

			else if constexpr (putils::reflection::has_attributes<Member>()) {
				if (ImGui::TreeNode(nameWithID)) {
					putils::reflection::for_each_attribute<Member>([&member](const char * name, const auto attr) {
						editAttribute(name, member.*attr);
					});
					ImGui::TreePop();
				}
			}

			else if constexpr (putils::is_function<Member>()) {
				displayAttribute(name, member);
			}

			else if constexpr (std::is_enum_v<Member>) {
				displayInColumns(name, [&] {
					imguiEnumCombo(id, member);
				});
			}
			else if constexpr (std::is_same_v<Member, bool>) {
				displayInColumns(name, [&] {
					ImGui::Checkbox(id, &member);
				});
			}
			else if constexpr (std::is_same_v<Member, int>) {
				displayInColumns(name, [&] {
					ImGui::PushItemWidth(-1.f);
					auto val = member;
					if (ImGui::InputInt(id, &val, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
						member = val;
					ImGui::PopItemWidth();
				});
			}
			else if constexpr (std::is_same_v<Member, unsigned int> || std::is_same_v<Member, size_t>) {
				displayInColumns(name, [&] {
					ImGui::PushItemWidth(-1.f);
					auto val = member;
					if (ImGui::InputScalar(id, sizeof(Member) == 64 ? ImGuiDataType_U64 : ImGuiDataType_U32, &val, nullptr, nullptr, nullptr, ImGuiInputTextFlags_EnterReturnsTrue))
						member = val;
					ImGui::PopItemWidth();
				});
			}
			else if constexpr (std::is_same_v<Member, float>) {
				displayInColumns(name, [&] {
					ImGui::PushItemWidth(-1.f);
					auto val = member;
					if (ImGui::InputFloat(id, &val, 0.f, 0.f, "%.6f", ImGuiInputTextFlags_EnterReturnsTrue))
						member = val;
					ImGui::PopItemWidth();
				});
			}
			else if constexpr (std::is_same_v<Member, double>) {
				displayInColumns(name, [&] {
					ImGui::PushItemWidth(-1.f);
					auto val = member;
					if (ImGui::InputDouble(id, &val, 0.0, 0.0, "%.6f", ImGuiInputTextFlags_EnterReturnsTrue))
						member = val;
					ImGui::PopItemWidth();
				});
			}
			else {
				displayInColumns(name, [&] {
					ImGui::Text("Unknown type");
				});
			}
		}

		template<typename Member>
		static void displayAttribute(const char * name, const Member & member) {
			const auto id = getID(name, member);
			const auto nameWithID = getNameWithID(name, member);

			if constexpr (detail::imgui::has_member_c_str<Member>()) {
				displayInColumns(name, [&] {
					ImGui::Text(member.c_str());
				});
			}
			// else if constexpr (std::is_same_v<Member, const char *>::value)
			// 	ImGui::LabelText(name, member);

			else if constexpr (putils::is_iterable<Member>()) {
				if (ImGui::TreeNode(nameWithID)) {
					int i = 0;
					for (const auto & val : member)
						displayAttribute(putils::string<64>("%d", i++), val);
					ImGui::TreePop();
				}
			}

			else if constexpr (std::is_same_v<Member, putils::Color>) {
				displayInColumns(name, [&] {
					const auto normalized = putils::toNormalizedColor(member);
					const ImVec4 col = { normalized.r, normalized.g, normalized.b, normalized.a };
					ImGui::ColorButton(id, col);
				});
			}
			else if constexpr (std::is_same_v<Member, putils::NormalizedColor>) {
				displayInColumns(name, [&] {
					const ImVec4 col = { member.r, member.g, member.b, member.a };
					ImGui::ColorButton(id, col);
				});
			}

			else if constexpr (putils::reflection::has_attributes<Member>()) {
				if (ImGui::TreeNode(nameWithID)) {
					putils::reflection::for_each_attribute<Member>([&member](const char * name, const auto attr) {
						displayAttribute(name, member.*attr);
					});
					ImGui::TreePop();
				}
			}

			else if constexpr (putils::is_function<Member>()) {
				if (member == nullptr)
					return;

				using Args = putils::function_arguments<Member>;

				if constexpr (std::is_default_constructible<Args>()) {
					if (ImGui::TreeNode(nameWithID)) {
						static Args args;

						if (ImGui::Button("Call"))
							std::apply(member, args);

						size_t i = 0;
						putils::tuple_for_each(args, [&](auto & arg) {
							editAttribute(putils::string<64>("Arg %zu", i), arg);
							++i;
						});

						using Ret = putils::function_return_type<Member>;
						if constexpr (!std::is_same<Ret, void>() && std::is_default_constructible<Ret>()) {
							static Ret ret;
							displayAttribute("Return value", ret);
						}

						ImGui::TreePop();
					}
				}
			}

			else {
				displayInColumns(name, [&] {
					if constexpr (std::is_enum_v<Member>)
						ImGui::Text(putils::magic_enum::enum_name(member).data());
					else if constexpr (std::is_same_v<Member, bool>)
						ImGui::Text(member ? "true" : "false");
					else if constexpr (std::is_same_v<Member, int>)
						ImGui::Text("%d", member);
					else if constexpr (std::is_same_v<Member, unsigned int> || std::is_same_v<Member, size_t>)
						ImGui::Text("%zu", member);
					else if constexpr (std::is_same_v<Member, float>)
						ImGui::Text("%f", member);
					else if constexpr (std::is_same_v<Member, double>)
						ImGui::Text("%d", member);
				});
			}
		}
	}

	template<typename T>
	void imguiEdit(T & obj) {
		putils::reflection::for_each_attribute<T>([&](const char * name, auto member) {
			detail::imgui::editAttribute(name, obj.*member);
		});
	}

	template<typename T>
	void imguiDisplay(const T & obj) {
		putils::reflection::for_each_attribute<T>([&](const char * name, const auto member) {
			detail::imgui::displayAttribute(name, obj.*member);
		});
	}
}