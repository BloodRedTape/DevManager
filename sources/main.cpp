#include <core/print.hpp>
#include <core/span.hpp>
#include "application.hpp"

int StraitXMain(Span<const char*> args){
    return Application().Run();
}
