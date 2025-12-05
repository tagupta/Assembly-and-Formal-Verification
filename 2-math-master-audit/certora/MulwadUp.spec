/*
* Verification of MulwadUp for Math masters
*/
// internal functions on a library or just simple internal functions can not be marked as external inside methods
// this requires harness contract => wrapper around a library

methods {
    function mulWadUp(uint256, uint256) external returns (uint256) envfree;
}

definition WAD() returns uint256 = 1000000000000000000;
// definition MAX_UINT256() returns uint256 = 0xffffffffffffffffffffffffffffffff;

//certoraRun ./certora/MulWadUp.conf
rule check_testMulWadUpFuzz(uint256 x, uint256 y) {
    require(x == 0 || y == 0 || y <= assert_uint256(max_uint256 / x), "Base condition");
    uint256 result = mulWadUp(x, y);
    mathint expected = x * y == 0 ? 0 : (x * y - 1) / WAD() + 1;
    assert(result == assert_uint256(expected));
}

invariant mulWadUpInvariant(uint256 x, uint256 y)
    mulWadUp(x, y) == assert_uint256(x * y == 0 ? 0 : (x * y - 1) / WAD() + 1)
    {
        preserved{
            require(x == 0 || y == 0 || y <= assert_uint256(max_uint256 / x), "Base condition");
        }
    }
