/*
* Verification of Sqrt for Math masters
*/
//compact code base of MathMaster
methods {
    // function mathMastersSqrt(uint256) external returns (uint256) envfree;
    // function uniSqrt(uint256) external returns (uint256) envfree;
    function solmateTopHalf(uint256) external returns(uint256) envfree;
    function mathMastersTopHalf(uint256)external returns (uint256)envfree;
}

// rule check_sqrtfuzz(uint256 x) {
//     assert(mathMastersSqrt(x) == uniSqrt(x));
// }

// 0xfffffe00000001
rule check_topHalfs(uint256 x) {
    assert(mathMastersTopHalf(x) == solmateTopHalf(x));
}