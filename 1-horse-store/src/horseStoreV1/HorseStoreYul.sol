// SPDX-License-Identifier: MIT
pragma solidity 0.8.20;

contract HorseStoreYul {
    uint256 numberOhHorses;

    function updateHorseNumber(uint256 newNumberOfHorses) external {
        uint256 slot;
        assembly {
            slot := numberOhHorses.slot
            sstore(slot, newNumberOfHorses)
        }
    }

    function readNumberOfHorses() external view returns (uint256) {
        uint256 slot;
        uint256 horses;
        assembly {
            horses := sload(slot)
            mstore(0, horses)
            return(0, 0x20)
        }
    }
}
