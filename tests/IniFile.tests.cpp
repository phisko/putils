#include <gtest/gtest.h>
#include "IniFile.hpp"

TEST(IniFile, parse) {
    std::stringstream s;
    s <<
R"([Section]
A=42
B="hello"
C=2.5
D ='hi'

[Section/Subsection]
A = 84

[OtherSection]
A= -42
)";

    putils::IniFile f;
    s >> f;

    EXPECT_EQ(f.sections.size(), 2);

    EXPECT_EQ(f.sections["Section"].values.size(), 4);
    EXPECT_EQ(f.sections["Section"].values["A"], "42");
    EXPECT_EQ(f.sections["Section"].values["B"], "hello");
    EXPECT_EQ(f.sections["Section"].values["C"], "2.5");
    EXPECT_EQ(f.sections["Section"].values["D"], "hi");

    EXPECT_EQ(f.sections["Section"].sections.size(), 1);
    EXPECT_EQ(f.sections["Section"].sections["Subsection"].values.size(), 1);
    EXPECT_EQ(f.sections["Section"].sections["Subsection"].values["A"], "84");
    EXPECT_EQ(f.sections["Section"].sections["Subsection"].sections.size(), 0);

    EXPECT_EQ(f.sections["OtherSection"].sections.size(), 0);
    EXPECT_EQ(f.sections["OtherSection"].values.size(), 1);
    EXPECT_EQ(f.sections["OtherSection"].values["A"], "-42");
}

TEST(IniFile, serialize) {
    const putils::IniFile ini{
        .sections = {
            {
                "Section",
                {
                    .values = {
                        { "A", "42", },
                        { "B", "hello" },
                        { "C", "2.5" },
                        { "D", "hi" }
                    },
                    .sections = {
                        {
                            "Subsection",
                            {
                                .values = {
                                    { "A", "84" }
                                }
                            }
                        }
                    }
                }
            },
            {
                "OtherSection",
                {
                    .values = {
                        { "A", "-42" }
                    }
                }
            }
        }
    };

    std::stringstream s;
    s << ini;

    EXPECT_EQ(s.str(),
R"([OtherSection]
A=-42

[Section]
A=42
B=hello
C=2.5
D=hi

[Section/Subsection]
A=84
)");
}