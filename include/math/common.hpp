#ifndef COMMON_HPP
#define COMMON_HPP
using real = double;
namespace Constant
{
	constexpr real Epsilon = 0.0000000000000000000001;
	constexpr real Pi = 3.141592653589793238463;
	constexpr real HalfPi = Constant::Pi / 2.0;
	constexpr real DoublePi = Constant::Pi * 2.0;
}
#endif