object "HorseStoreYul"{
    code {
        //Contract deployment
        datacopy(0, dataoffset("runtime"), datasize("runtime"))
        return(0, datasize("runtime"))
    }

    object "runtime"{
        code {
            //function dispatcher
            switch selector()
            //updateHorseNumber
            case 0xcdfead2e {
                storeNumber(decodeAsUnit(0))
            }
            // readNumberOfHorses
            case 0xe026c017 {
                returnUint(readNumber())
            }
            default {
                revert(0,0)
            }

            function readNumber() -> storedNumber {
                storedNumber := sload(0)
            }
            
            function storeNumber(newNumber){
                sstore(0, newNumber)
            }
            /*decoding function */
            function selector() -> s {
                s:= div(calldataload(0), 0x100000000000000000000000000000000000000000000000000000000)
            }

            function decodeAsUnit(offset) -> v {
                let positionInCalldata := add(4, mul(offset, 0x20))
                if lt(calldatasize(), add(positionInCalldata, 0x20)){
                    revert(0,0)
                }
                v:= calldataload(positionInCalldata)
            }

            function returnUint(v){
                mstore(0,v)
                return(0, 0x20)
            }

        }
    }
}