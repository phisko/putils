// gtest
#include <gtest/gtest.h>

// putils
#include "reflection_helpers/runtime_helper.hpp"

namespace {
    struct Reflectible {
        int i = 42;
        std::string s = "hello";

        struct Nested {
            std::string s = "hi";
            int i = 84;
        };

        Nested nested;
        Nested nestedArray[8];
        std::vector<Nested> nestedVector = {
            { .s = "ho", .i = 21 },
            { .s = "foo", .i = -42 }
        };
        std::unordered_map<int, Nested> nestedMap = {
            { 0, { .s = "bar", .i = 1 }},
            { 1, { .s = "bar", .i = 2 }}
        };
    };
}

#define refltype Reflectible
putils_reflection_info{
    putils_reflection_attributes(
            putils_reflection_attribute(i),
            putils_reflection_attribute(s),
            putils_reflection_attribute(nested),
            putils_reflection_attribute(nestedArray),
            putils_reflection_attribute(nestedVector),
            putils_reflection_attribute(nestedMap)
    );
};
#undef refltype

#define refltype Reflectible::Nested
putils_reflection_info{
    putils_reflection_attributes(
            putils_reflection_attribute(i),
            putils_reflection_attribute(s)
    );
};
#undef refltype

TEST(runtime_helper, getAttributes) {
    const auto &attributes = putils::reflection::runtime::getAttributes<Reflectible>();
    EXPECT_EQ(attributes.map.size(), 6);
    EXPECT_NE(attributes.map.find("i"), attributes.map.end());
    EXPECT_NE(attributes.map.find("s"), attributes.map.end());
    EXPECT_NE(attributes.map.find("nested"), attributes.map.end());
    EXPECT_NE(attributes.map.find("nestedArray"), attributes.map.end());
    EXPECT_NE(attributes.map.find("nestedVector"), attributes.map.end());
    EXPECT_NE(attributes.map.find("nestedMap"), attributes.map.end());
    EXPECT_EQ(attributes.map.find("foo"), attributes.map.end());
}

TEST(runtime_helper, findAttributeInMap) {
    const auto & attributes = putils::reflection::runtime::getAttributes<Reflectible>();
    EXPECT_NE(putils::reflection::runtime::findAttribute(attributes, "i"), nullptr);
    EXPECT_NE(putils::reflection::runtime::findAttribute(attributes, "s"), nullptr);
    EXPECT_NE(putils::reflection::runtime::findAttribute(attributes, "nested"), nullptr);
    EXPECT_EQ(putils::reflection::runtime::findAttribute(attributes, "foo"), nullptr);
}

TEST(runtime_helper, findAttribute) {
    EXPECT_NE(putils::reflection::runtime::findAttribute<Reflectible>("i"), nullptr);
    EXPECT_NE(putils::reflection::runtime::findAttribute<Reflectible>("s"), nullptr);
    EXPECT_NE(putils::reflection::runtime::findAttribute<Reflectible>("nested"), nullptr);
    EXPECT_EQ(putils::reflection::runtime::findAttribute<Reflectible>("foo"), nullptr);
}

TEST(runtime_helper, findAttributeNested) {
    EXPECT_NE(putils::reflection::runtime::findAttribute<Reflectible>("nested.i"), nullptr);
    EXPECT_NE(putils::reflection::runtime::findAttribute<Reflectible>("nested.s"), nullptr);
    EXPECT_EQ(putils::reflection::runtime::findAttribute<Reflectible>("nested.zob"), nullptr);
}

TEST(runtime_helper, findAttributeNestedCustomSeparator) {
    EXPECT_NE(putils::reflection::runtime::findAttribute<Reflectible>("nested/i", "/"), nullptr);
    EXPECT_NE(putils::reflection::runtime::findAttribute<Reflectible>("nested/s", "/"), nullptr);
    EXPECT_EQ(putils::reflection::runtime::findAttribute<Reflectible>("nested/zob", "/"), nullptr);
}

TEST(runtime_helper, intAttribute) {
    const auto attr = putils::reflection::runtime::findAttribute<Reflectible>("i");

    EXPECT_EQ(attr->offset, putils::member_offset(&Reflectible::i));
    EXPECT_EQ(attr->size, sizeof(int));
    EXPECT_EQ(attr->attributes.map.size(), 0);
    EXPECT_EQ(attr->arrayHelper, std::nullopt);
    EXPECT_EQ(attr->mapHelper, std::nullopt);
}

TEST(runtime_helper, stringAttribute) {
    const Reflectible obj;
    const auto attr = putils::reflection::runtime::findAttribute<Reflectible>("s");

    EXPECT_EQ(attr->offset, putils::member_offset(&Reflectible::s));
    EXPECT_EQ(attr->size, sizeof(std::string));
    EXPECT_EQ(attr->attributes.map.size(), 0);

    EXPECT_EQ(attr->arrayHelper->getSize(&obj.s), obj.s.size());
    for (int i = 0; i < obj.s.size(); ++i)
        EXPECT_EQ(attr->arrayHelper->getElement(&obj.s, i), &obj.s[i]);

    int i = 0;
    attr->arrayHelper->forEach(&obj.s, [&](void * element) {
        auto c = (char *)element;
        EXPECT_EQ(c, &obj.s[i]);
        ++i;
    });

    EXPECT_EQ(attr->mapHelper, std::nullopt);
}

TEST(runtime_helper, reflectibleAttribute) {
    const Reflectible obj;
    {
        const auto attr = putils::reflection::runtime::findAttribute<Reflectible>("nested");
        EXPECT_EQ(attr->offset, putils::member_offset(&Reflectible::nested));
        EXPECT_EQ(attr->size, sizeof(Reflectible::Nested));
        EXPECT_EQ(attr->attributes.map.size(), 2);
        EXPECT_NE(attr->attributes.map.find("i"), attr->attributes.map.end());
        EXPECT_NE(attr->attributes.map.find("s"), attr->attributes.map.end());
        EXPECT_EQ(attr->attributes.map.find("foo"), attr->attributes.map.end());
        EXPECT_EQ(attr->arrayHelper, std::nullopt);
        EXPECT_EQ(attr->mapHelper, std::nullopt);
    }

    {
        const auto attr = putils::reflection::runtime::findAttribute<Reflectible>("nested.i");
        EXPECT_EQ(attr->offset, putils::member_offset(&Reflectible::Nested::i));
        EXPECT_EQ(attr->size, sizeof(int));
        EXPECT_EQ(attr->attributes.map.size(), 0);
        EXPECT_EQ(attr->arrayHelper, std::nullopt);
        EXPECT_EQ(attr->mapHelper, std::nullopt);
    }

    {
        const auto attr = putils::reflection::runtime::findAttribute<Reflectible>("nested.s");

        EXPECT_EQ(attr->offset, putils::member_offset(&Reflectible::Nested::s));
        EXPECT_EQ(attr->size, sizeof(std::string));
        EXPECT_EQ(attr->attributes.map.size(), 0);

        EXPECT_EQ(attr->arrayHelper->getSize(&obj.nested.s), obj.nested.s.size());
        for (int i = 0; i < obj.nested.s.size(); ++i)
            EXPECT_EQ(attr->arrayHelper->getElement(&obj.nested.s, i), &obj.nested.s[i]);

        int i = 0;
        attr->arrayHelper->forEach(&obj.nested.s, [&](void *element) {
            auto c = (char *) element;
            EXPECT_EQ(c, &obj.nested.s[i]);
            ++i;
        });

        EXPECT_EQ(attr->mapHelper, std::nullopt);
    }
}

TEST(runtime_helper, arrayAttribute) {
    const Reflectible obj;

    const auto attr = putils::reflection::runtime::findAttribute<Reflectible>("nestedArray");
    EXPECT_EQ(attr->size, sizeof(Reflectible::nestedArray));
    EXPECT_EQ(attr->offset, putils::member_offset(&Reflectible::nestedArray));
    EXPECT_EQ(attr->attributes.map.size(), 0);
    EXPECT_EQ(attr->arrayHelper->getSize(&obj.nestedArray), putils::lengthof(obj.nestedArray));

    for (int i = 0; i < putils::lengthof(obj.nestedArray); ++i)
        EXPECT_EQ(attr->arrayHelper->getElement(&obj.nestedArray, i), &obj.nestedArray[i]);

    int i = 0;
    attr->arrayHelper->forEach(&obj.nestedArray, [&](void * element) {
        const auto * nested = (Reflectible::Nested*)element;
        EXPECT_EQ(nested->s, obj.nestedArray[i].s);
        EXPECT_EQ(nested->i, obj.nestedArray[i].i);
        ++i;
    });

    EXPECT_EQ(attr->arrayHelper->elementAttributes.map.size(), 2);
    EXPECT_NE(attr->arrayHelper->elementAttributes.map.find("i"), attr->arrayHelper->elementAttributes.map.end());
    EXPECT_NE(attr->arrayHelper->elementAttributes.map.find("s"), attr->arrayHelper->elementAttributes.map.end());
    EXPECT_EQ(attr->arrayHelper->elementAttributes.map.find("foo"), attr->arrayHelper->elementAttributes.map.end());

    EXPECT_EQ(attr->mapHelper, std::nullopt);
}

TEST(runtime_helper, vectorAttribute) {
    const Reflectible obj;

    const auto attr = putils::reflection::runtime::findAttribute<Reflectible>("nestedVector");
    EXPECT_EQ(attr->size, sizeof(Reflectible::nestedVector));
    EXPECT_EQ(attr->offset, putils::member_offset(&Reflectible::nestedVector));
    EXPECT_EQ(attr->attributes.map.size(), 0);
    EXPECT_EQ(attr->arrayHelper->getSize(&obj.nestedVector), obj.nestedVector.size());

    for (int i = 0; i < obj.nestedVector.size(); ++i)
        EXPECT_EQ(attr->arrayHelper->getElement(&obj.nestedVector, i), &obj.nestedVector[i]);

    int i = 0;
    attr->arrayHelper->forEach(&obj.nestedVector, [&](void * element) {
        const auto * nested = (Reflectible::Nested*)element;
        EXPECT_EQ(nested->s, obj.nestedVector[i].s);
        EXPECT_EQ(nested->i, obj.nestedVector[i].i);
        ++i;
    });

    EXPECT_EQ(attr->arrayHelper->elementAttributes.map.size(), 2);
    EXPECT_NE(attr->arrayHelper->elementAttributes.map.find("i"), attr->arrayHelper->elementAttributes.map.end());
    EXPECT_NE(attr->arrayHelper->elementAttributes.map.find("s"), attr->arrayHelper->elementAttributes.map.end());
    EXPECT_EQ(attr->arrayHelper->elementAttributes.map.find("foo"), attr->arrayHelper->elementAttributes.map.end());

    EXPECT_EQ(attr->mapHelper, std::nullopt);
}

TEST(runtime_helper, mapAttribute) {
    const Reflectible obj;

    const auto attr = putils::reflection::runtime::findAttribute<Reflectible>("nestedMap");
    EXPECT_EQ(attr->size, sizeof(Reflectible::nestedMap));
    EXPECT_EQ(attr->offset, putils::member_offset(&Reflectible::nestedMap));
    EXPECT_EQ(attr->attributes.map.size(), 0);
    EXPECT_EQ(attr->arrayHelper, std::nullopt);
    EXPECT_EQ(attr->mapHelper->getSize(&obj.nestedMap), obj.nestedMap.size());

    for (const auto & [key, value] : obj.nestedMap)
        EXPECT_EQ(attr->mapHelper->getValue(&obj.nestedMap, putils::toString(key).c_str()), &value);

    attr->mapHelper->forEach(&obj.nestedMap, [&](const void * key, void * value) {
        const auto k = (int *)key;
        EXPECT_EQ(&obj.nestedMap.at(*k), value);
    });

    EXPECT_EQ(attr->mapHelper->keyAttributes.map.size(), 0);

    EXPECT_EQ(attr->mapHelper->valueAttributes.map.size(), 2);
    EXPECT_NE(attr->mapHelper->valueAttributes.map.find("i"), attr->mapHelper->valueAttributes.map.end());
    EXPECT_NE(attr->mapHelper->valueAttributes.map.find("s"), attr->mapHelper->valueAttributes.map.end());
    EXPECT_EQ(attr->mapHelper->valueAttributes.map.find("foo"), attr->mapHelper->valueAttributes.map.end());
}
