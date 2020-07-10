#pragma once 

#define UNIVERSAL_FUNCTOR(func) [](auto && ... args) { func(FWD(args)...); }
