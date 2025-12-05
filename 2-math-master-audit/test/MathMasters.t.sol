// SPDX-License-Identifier: AGPL-3.0-only
pragma solidity ^0.8.3;

import {Base_Test, console2} from "./Base_Test.t.sol";
import {CompactCodeBase} from 'test/CompactCodeBase.sol';
import {MathMasters} from "src/MathMasters.sol";

contract MathMastersTest is Base_Test {
    function testMulWad() public {
        assertEq(MathMasters.mulWad(2.5e18, 0.5e18), 1.25e18);
        assertEq(MathMasters.mulWad(3e18, 1e18), 3e18);
        assertEq(MathMasters.mulWad(369, 271), 0);
    }

    function testMulWadFuzz(uint256 x, uint256 y) public pure {
        // Ignore cases where x * y overflows.
        unchecked {
            if (x != 0 && (x * y) / x != y) return;
        }
        assert(MathMasters.mulWad(x, y) == (x * y) / 1e18);
    }

    function testMulWadUp() public {
        assertEq(MathMasters.mulWadUp(2.5e18, 0.5e18), 1.25e18);
        assertEq(MathMasters.mulWadUp(3e18, 1e18), 3e18);
        assertEq(MathMasters.mulWadUp(369, 271), 1);
    }

    function testMulWadUpFuzz(uint256 x, uint256 y) public {
        // We want to skip the case where x * y would overflow.
        // Since Solidity 0.8.0 checks for overflows by default,
        // we cannot just multiply x and y as this could revert.
        // Instead, we can ensure x or y is 0, or
        // that y is less than or equal to the maximum uint256 value divided by x
        if (x == 0 || y == 0 || y <= type(uint256).max / x) {
            uint256 result = MathMasters.mulWadUp(x, y);
            uint256 expected = x * y == 0 ? 0 : (x * y - 1) / 1e18 + 1;
            assertEq(result, expected);
        }
        // If the conditions for x and y are such that x * y would overflow,
        // this function will simply not perform the assertion.
        // In a testing context, you might want to handle this case differently,
        // depending on whether you want to consider such an overflow case as passing or failing.
    }

    function testMulWadUpFuzzInputOne() public {
        uint256 x = 16877076170751140273593;
        uint256 y = 10163048856434588128724;
        if(y <= type(uint256).max / x){
            uint256 result = MathMasters.mulWadUp(x, y);
            uint256 expectedValue = (x * y - 1) / 1e18 + 1;
            assertEq(result, expectedValue);
        }
    }

    function testMulWadUpFuzzInputTwo() public {
        uint256 x = 228763264180973796704512519836425781249;
        uint256 y = 170141183460469231731687303715884105728;
        if(y <= type(uint256).max / x){
            uint256 result = MathMasters.mulWadUp(x, y);
            uint256 expectedValue = (x * y - 1) / 1e18 + 1;
            assertEq(result, expectedValue);
        }
    }

    function testMulWadUpFuzzInputThree() external {
        uint256 x=1999999998;
        uint256 y=1000000001;
        if(y <= type(uint256).max / x){
            uint256 result = MathMasters.mulWadUp(x, y);
            uint256 expectedValue = (x * y - 1) / 1e18 + 1;
            assertEq(result, expectedValue);
        }
    }

    //halmos --function check_testMulWadUpFuzz_halmos
    function check_testMulWadUpFuzz_halmos(uint256 x, uint256 y) external {
        if (x == 0 || y == 0 || y <= type(uint256).max / x) {
            uint256 result = MathMasters.mulWadUp(x, y);
            uint256 expected = x * y == 0 ? 0 : (x * y - 1) / 1e18 + 1;
            assert(result == expected);
        }
    }

    function testSqrt() public {
        assertEq(MathMasters.sqrt(0), 0);
        assertEq(MathMasters.sqrt(1), 1);
        assertEq(MathMasters.sqrt(2704), 52);
        assertEq(MathMasters.sqrt(110889), 333);
        assertEq(MathMasters.sqrt(32239684), 5678);
        assertEq(MathMasters.sqrt(type(uint256).max), 340282366920938463463374607431768211455);
    }

    function testSqrtFuzzUni(uint256 x) public pure {
        assert(MathMasters.sqrt(x) == uniSqrt(x));
    }

    function check_testSqrtFuzzUni(uint256 x) external pure{
        assert(MathMasters.sqrt(x) == uniSqrt(x));
    }

    //halmos --function testSqrtFuzzSolmate
    function testSqrtFuzzSolmate(uint256 x) public pure {
        assert(MathMasters.sqrt(x) == solmateSqrt(x));
    }
  
    function testSqrtFuzzSolmateInputOne() public pure {
        uint256 x = 0xfffffe00000001;
        assertEq(MathMasters.sqrt(x), solmateSqrt(x));
    }

    function testTopHalfsOfMathMastersAndSolMate() external {
        uint256 x = 0xffff4300100000;
        CompactCodeBase compactCode = new CompactCodeBase();
        uint256 z_mathMasters = compactCode.mathMastersTopHalf(x);
        uint256 z_solMate = compactCode.solmateTopHalf(x);
        assertEq(z_mathMasters, z_solMate, "Equal assert failed");
    }

    //halmos --function check_testTopHalfsOfMathMastersAndSolMate_halmos
    function check_testTopHalfsOfMathMastersAndSolMate_halmos(uint256 x)external{
        CompactCodeBase compactCode = new CompactCodeBase();
        uint256 z_mathMasters = compactCode.mathMastersTopHalf(x);
        uint256 z_solMate = compactCode.solmateTopHalf(x);
        assert(z_mathMasters == z_solMate);
    }

// Counterexample: 
//     p_x_uint256_75fc0cd_00 = 0xffff9c0000000000000000
// Counterexample: 
//     p_x_uint256_75fc0cd_00 = 0xffff9c00000000000000000000000000000000
// Counterexample: 
//     p_x_uint256_75fc0cd_00 = 0xffff6a00000000000000000000000000000000000000000000000000000000
// Counterexample: 
//     p_x_uint256_75fc0cd_00 = 0xffff6a000000800000000000000000000000000000000000000000
// Counterexample: 
//     p_x_uint256_75fc0cd_00 = 0xffff6a0000000000000000000000000000000000000000
// Counterexample: 
//     p_x_uint256_75fc0cd_00 = 0xffff4300100000 0xffff4300100000
// Counterexample: 
//     p_x_uint256_75fc0cd_00 = 0xffff9f008000004000000000000000
}
