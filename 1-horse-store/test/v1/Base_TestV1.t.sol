// SPDX-License-Identifier: MIT
pragma solidity 0.8.20;

import {Test, console2} from "forge-std/Test.sol";
import {HorseStore} from "src/horseStoreV1/HorseStore.sol";
import {IHorseStore} from "src/horseStoreV1/IHorseStore.sol";

abstract contract Base_TestV1 is Test {
    IHorseStore public horseStore;

    bytes yulCode = "5f3560e01c8063cdfead2e1460245763e026c01714601b575f80fd5b5f545f5260205ff35b602436106032576004355f55005b5f80fd";

    function setUp() public virtual {
        horseStore = IHorseStore(address(new HorseStore()));
    }

    function testReadValue() public view {
        uint256 initialValue = horseStore.readNumberOfHorses();
        assertEq(initialValue, 0);
    }

    function testWriteValue(uint256 x) public {
        //uint256 x
        horseStore.updateHorseNumber(x);
        assertEq(horseStore.readNumberOfHorses(), x);
    }
}
