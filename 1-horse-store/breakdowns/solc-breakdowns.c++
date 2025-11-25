// 0x6080604052   
// Contract creation code
// 0x608060405234801561000f575f80fd5b506101438061001d5f395ff3 fe 
// Run time code 608060405234801561000f575f80fd5b5060043610610034575f3560e01c8063cdfead2e14610038578063e026c01714610054575b5f80fd5b610052600480360381019061004d91906100ba565b610072565b005b61005c61007b565b60405161006991906100f4565b60405180910390f35b805f8190555050565b5f8054905090565b5f80fd5b5f819050919050565b61009981610087565b81146100a3575f80fd5b50565b5f813590506100b481610090565b92915050565b5f602082840312156100cf576100ce610083565b5b5f6100dc848285016100a6565b91505092915050565b6100ee81610087565b82525050565b5f6020820190506101075f8301846100e5565b9291505056 fe
//Metadata
// a26469706673582212202ed6fced62a8e6889a76fd12d6b9e7364d16aefe1211f170de2ce08e540beb4464736f6c63430008140033

//3 sections
//1. Contract creation code
//2. Runtime code
//3. Metadata

//1. Contract creation code
// Free memory pointer
PUSH1 0x80 //[0x80]
PUSH1 0x40 //[0x40, 0x80][offset, value] 
MSTORE //[], [m:0x40 = 0x80]

//What's this chunk is doing here?
//@note if someone sent value with this call, revert
//else jump to 0x000f to continue execution
CALLVALUE //[msg.value]
DUP1//[msg.value, msg.value]
ISZERO //[msg.value == 0, msg.value]
PUSH2 0x000f // [0x000f, msg.value == 0, msg.value]
JUMPI //[msg.value], If we don't jump, msg.value is not equal to 0
PUSH0 //[0x00, msg.value]
DUP1 //[0x00, 0x00, msg.value],[offset, size]
REVERT //[msg.value]

//Jumpdest if msg.value == 0
//Sticks the run time code on chain
JUMPDEST //[msg.value]
POP //[]
PUSH2 0x0143 //[0x0143]
DUP1 //[0x0143, 0x0143]
PUSH2 0x001d //[0x001d, 0x0143, 0x0143]
PUSH0 //[0x00, 0x001d, 0x0143, 0x0143], [dstOffset, offset, size]
CODECOPY //[0x0143], [m:0x00 => runtime code, m:0x40 => 0x80]
PUSH0 //[0x00, 0x0143], [offset, size], return data from memory at index 0x00 of size 0x0143
RETURN //[]
INVALID //[] => End of contract creation code

//2. Runtime code
//Entry point of all calls
//Free memory pointer
PUSH1 0x80
PUSH1 0x40
MSTORE //[m:0x40 = 0x80]

//Checking for msg.value, if given revert
CALLVALUE //[msg.value]
DUP1 //[msg.value, msg.value]
ISZERO //[msg.value == 0, msg.value]
PUSH2 0x000f//[0x000f, msg.value == 0, msg.value]
JUMPI //[msg.value]
//Jump to continue
PUSH0 //[0x00, msg.value]
DUP1 //[0x00, 0x00, msg.value], [offset,size]
REVERT //[msg.value]

//continue::
//Jumped here as msg.value == 0
//If calldata is too small like less than 4 bytes, we will jump
//Checking to see if there is enough bytes for function selector
JUMPDEST //0x000f,  [msg.value]
POP //[]
PUSH1 0x04 //[0x04]
CALLDATASIZE //[calldata_size, 0x04]
LT//[calldata_size < 0x04], [1/0]
PUSH2 0x0034 //[0x0034, calldata_size < 0x04]. 52
JUMPI //[]

//Fuction dispatching in solidity
PUSH0 //[0x00], byte offset in the calldata, read 32-byte value 
CALLDATALOAD //[calldata]
PUSH1 0xe0 //[0xe0, calldata], shift calldata by 224 bits to get top 32 bits (4*8)
SHR //[function seletor], calldata[0:4]

//Dispatching for upgateHorseNumber - 1
DUP1 //[function seletor, function seletor]
PUSH4 0xcdfead2e //[0xcdfead2e,function seletor,function seletor], updateHorseNumber
EQ //[0xcdfead2e == function seletor, function seletor]
PUSH2 0x0038 //[0x0038, 0xcdfead2e == function seletor, function seletor]
JUMPI //[function seletor]

//function dispatching for readNumberOfHorses-1
DUP1 //[function seletor, function seletor] //Could there is a way to avoid this dup?
PUSH4 0xe026c017 //[0xe026c017, function seletor, function seletor], readNumberOfHorses
EQ //[0xe026c017 == function seletor, function seletor]
PUSH2 0x0054 //[0x0054, 0xe026c017 == function seletor, function seletor]
JUMPI //[function seletor]

//Calldatasize < 4, jump here
JUMPDEST //[]
PUSH0//[0x00]
DUP1 //[0x00, 0x00], [offset, size]
REVERT //[]

//Update Horse number - 2
JUMPDEST //[function seletor]
PUSH2 0x0052 //[0x0052, function seletor]
PUSH1 0x04 //[0x04, 0x0052, function seletor]
DUP1 //[0x04, 0x04, 0x0052, function seletor]
CALLDATASIZE //[calldata_size, 0x04, 0x04, 0x0052, function seletor] - At this stage, it means that the size of calldata size is atleast more than 4 bytes
SUB //[calldata_size - 0x04, 0x04, 0x0052, function seletor]
DUP2 //[0x04, calldata_size - 0x04, 0x04, 0x0052, function seletor]
ADD //[calldata_size, 0x04, 0x0052, function seletor]
SWAP1 //[0x04, calldata_size, 0x0052, function seletor]
PUSH2 0x004d //[0x004d, 0x04, calldata_size, 0x0052, function seletor]
SWAP2 //[calldata_size, 0x04, 0x004d, 0x0052,function seletor]
SWAP1 //[0x04, calldata_size, 0x004d, 0x0052,function seletor]
PUSH2 0x00ba //[0x00ba, 0x04, calldata_size, 0x004d, 0x0052,function seletor]
JUMP //[0x04, calldata_size, 0x004d, 0x0052,function seletor]

//UpdateHorseNumber - 12
JUMPDEST //[calldata_32bytes_after_selector, 0x0052, function seletor]
PUSH2 0x0072 //[0x0072, calldata_32bytes_after_selector, 0x0052, function seletor]
JUMP //[calldata_32bytes_after_selector, 0x0052, function seletor]

//UpdateHorseNumber - 14
JUMPDEST //[function seletor]
STOP //[function seletor]

//Jump for reading horse number
//readNumberOfHorses-2
JUMPDEST //[function seletor]
PUSH2 0x005c //[0x005c, function seletor]
PUSH2 0x007b //[0x007b, 0x005c, function seletor]
JUMP //[0x005c, function seletor]

//readNumberOfHorses-4
JUMPDEST //[value, function seletor]
PUSH1 0x40 //[0x40, value, function seletor]
MLOAD //[free_memory_pointer (0x80),value, function seletor]
PUSH2 0x0069 //[0x0069, free_memory_pointer (0x80),value, function seletor]
SWAP2 //[value, free_memory_pointer (0x80), 0x0069, function seletor]
SWAP1 //[free_memory_pointer (0x80), value, 0x0069, function seletor]
PUSH2 0x00f4 //[0x00f4, free_memory_pointer (0x80), value, 0x0069, function seletor]
JUMP

//readNumberOfHorses - 10
JUMPDEST //[new_free_memory_pointer_location, 0x0069, function seletor]
PUSH1 0x40 //[0x40, new_free_memory_pointer_location, 0x0069, function seletor], //Offset
MLOAD //[value_0x80, new_free_memory_pointer_location, 0x0069, function seletor]
DUP1 //[value_0x80, value_0x80, new_free_memory_pointer_location, 0x0069, function seletor]
SWAP2 //[new_free_memory_pointer_location, value_0x80, value_0x80, 0x0069, function seletor]
SUB//[0x20, value_0x80, 0x0069, function seletor]
SWAP1 //[value_0x80, 0x20, 0x0069, function seletor] [offset, size,  0x0069, function seletor]
RETURN //[0x0069, function seletor], returns value of 32 bytes stored at 0x80 memory pointer

//UpdateHorseNumber - 13
JUMPDEST //[calldata_32bytes_after_selector, 0x0052, function seletor]
DUP1 //[calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x0052, function seletor]
PUSH0 //[0x00, calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x0052, function seletor]
DUP2 //[calldata_32bytes_after_selector, 0x00, calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x0052, function seletor]
SWAP1 //[0x00, calldata_32bytes_after_selector, calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x0052, function seletor] //key, value
SSTORE //[calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x0052, function seletor], storage: 0 => calldata_32bytes_after_selector
POP //[calldata_32bytes_after_selector, 0x0052, function seletor]
POP //[0x0052, function seletor]
JUMP //[function seletor]

//readNumberOfHorses-3
JUMPDEST //[0x005c, function seletor]
PUSH0 //[0x00, 0x005c, function seletor]
DUP1 //[0x00, 0x00, 0x005c, function seletor]
SLOAD //[value, 0x00, 0x005c, function seletor]
SWAP1 //[0x00, value, 0x005c, function seletor]
POP //[value, 0x005c, function seletor]
SWAP1 //[0x005c, value, function seletor]
JUMP //[value, function seletor]

//Jump here if size of calldata is less than 32 bytes excluding function selector
JUMPDEST //[0x00ce, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
PUSH0 //[0x00, 0x00ce, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
DUP1 //[0x00, 0x00, 0x00ce, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
REVERT //[0x00ce, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
//Revert if calldata size is less than 36 bytes


//UpdateHorseNumber - 7
//readNumberOfHorses - 7
JUMPDEST //[calldata_32bytes_after_selector, 0x0099, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
//[value, 0x00ee, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]]
PUSH0 //[0x00, calldata_32bytes_after_selector, 0x0099, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
//[0x00, value, 0x00ee, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
DUP2 //[calldata_32bytes_after_selector, 0x00, calldata_32bytes_after_selector, 0x0099, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
//[value, 0x00, value, 0x00ee, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
SWAP1 //[0x00, calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x0099, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
//[0x00, value, value, 0x00ee, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
POP//[calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x0099, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
//[value, value, 0x00ee, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
SWAP2 //[0x0099, calldata_32bytes_after_selector, calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
//[0x00ee, value, value, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
SWAP1 //[calldata_32bytes_after_selector, 0x0099, calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
//[value, 0x00ee, value, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
POP //[0x0099, calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
//[0x00ee, value, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
JUMP //[calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
//[value, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]

//UpdateHorseNumber - 6
JUMPDEST //[calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
PUSH2 0x0099 //[0x0099, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
DUP2 //[calldata_32bytes_after_selector, 0x0099, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
PUSH2 0x0087 //[0x0087, calldata_32bytes_after_selector, 0x0099, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
JUMP //[calldata_32bytes_after_selector, 0x0099, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]


//UpdateHorseNumber - 8
JUMPDEST //[calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
DUP2 //[calldata_32bytes_after_selector, calldata_32bytes_after_selector, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
EQ//[1, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
PUSH2 0x00a3 //[0x00a3, 1, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
JUMPI //[calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]

//Unreachable code??
PUSH0
DUP1
REVERT

//UpdateHorseNumber - 9
JUMPDEST //[calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
POP //[0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
JUMP //[calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]

//UpdateHorseNumber - 5
//Grab the calldata to update the number of horses with
JUMPDEST //[0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
PUSH0 //[0x00, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
DUP2 //[0x04, 0x00, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
CALLDATALOAD //[calldata_32bytes_after_selector, 0x00, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
SWAP1 //[0x00, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
POP //[calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
PUSH2 0x00b4 //[0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
DUP2 //[calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
PUSH2 0x0090 //[0x0090, calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
JUMP //[calldata_32bytes_after_selector, 0x00b4, calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]

//update horse number - 10
JUMPDEST //[calldata_32bytes_after_selector, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
SWAP3 //[0x00dc, 0x04, calldata_size, calldata_32bytes_after_selector, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
SWAP2 //[calldata_size, 0x04, 0x00dc, calldata_32bytes_after_selector, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
POP //[0x04, 0x00dc, calldata_32bytes_after_selector, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
POP //[0x00dc, calldata_32bytes_after_selector, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
JUMP //[calldata_32bytes_after_selector, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]

//Jump from update horse number - 3
//Check to see if there is a value to update the horse number to?
JUMPDEST //[0x04, calldata_size, 0x004d, 0x0052,function seletor]
PUSH0 //[0x00, 0x04, calldata_size, 0x004d, 0x0052,function seletor]
PUSH1 0x20 //[0x20, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
DUP3 //[0x04, 0x20, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
DUP5 //[calldata_size, 0x04, 0x20, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
SUB //[calldata_size - 0x04, 0x20, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]

//is there more calldata than just the function selector?
SLT //[calldata_size - 0x04 < 0x20, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
ISZERO //[calldata_size - 0x04 < 0x20 == 0, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
PUSH2 0x00cf //[0x00cf, more_to_calldata_than_just_function_selector?, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
JUMPI //[0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
PUSH2 0x00ce //[0x00ce, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor], means the calldata size is less than 36 bytes
PUSH2 0x0083 //[0x0083, 0x00ce, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
JUMP //[0x00ce, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
JUMPDEST

//Update Horse Number - 4
//If there is more to calldata than just function selector?
JUMPDEST //[0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
PUSH0 //[0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
PUSH2 0x00dc //[0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
DUP5 //[calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
DUP3 //[0x00, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
DUP6 //[0x04, 0x00, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
ADD //[0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
PUSH2 0x00a6 //[0x00a6, 0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
JUMP //[0x04, calldata_size, 0x00dc, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]


//update horse number - 11
JUMPDEST //[calldata_32bytes_after_selector, 0x00, 0x00, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
SWAP2 //[0x00, 0x00, calldata_32bytes_after_selector, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
POP //[0x00, calldata_32bytes_after_selector, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
POP //[calldata_32bytes_after_selector, 0x04, calldata_size, 0x004d, 0x0052, function seletor]
SWAP3 //[0x004d, 0x04, calldata_size, calldata_32bytes_after_selector, 0x0052, function seletor]
SWAP2 //[calldata_size, 0x04, 0x004d, calldata_32bytes_after_selector, 0x0052, function seletor]
POP //[0x04, 0x004d, calldata_32bytes_after_selector, 0x0052, function seletor]
POP //[0x004d, calldata_32bytes_after_selector, 0x0052, function seletor]
JUMP //[calldata_32bytes_after_selector, 0x0052, function seletor]

//readNumberOfHorses - 6
JUMPDEST //[value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
PUSH2 0x00ee //[0x00ee, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
DUP2 //[value, 0x00ee, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
PUSH2 0x0087 //[0x0087, value, 0x00ee, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
JUMP //[value, 0x00ee, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]]

//readNumberOfHorses - 8
JUMPDEST //[value, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
DUP3 //[free_memory_pointer (0x80), value, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
MSTORE //[value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor], [m:0x80 => value]
POP //[free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
POP //[0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
JUMP

//readNumberOfHorses - 5
JUMPDEST //[free_memory_pointer (0x80), value, 0x0069, function seletor]
PUSH0 //[0x00, free_memory_pointer (0x80), value, 0x0069, function seletor]
PUSH1 0x20 //[0x20, 0x00, free_memory_pointer (0x80), value, 0x0069, function seletor]
DUP3 //[free_memory_pointer (0x80), 0x20, 0x00, free_memory_pointer (0x80), value, 0x0069, function seletor]
ADD // [new_free_memory_pointer_location, 0x00, free_memory_pointer (0x80), value, 0x0069, function seletor]
SWAP1 // [0x00, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
POP // [new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
PUSH2 0x0107 //[0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
PUSH0 //[0x00, 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
DUP4 //[free_memory_pointer (0x80),0x00, 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
ADD //[free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
DUP5 //[value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
PUSH2 0x00e5 //[0x00e5, value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
JUMP //[value, free_memory_pointer (0x80), 0x0107, new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]

//readNumberOfHorses - 9
JUMPDEST //[new_free_memory_pointer_location, free_memory_pointer (0x80), value, 0x0069, function seletor]
SWAP3//[0x0069, free_memory_pointer (0x80),value,  new_free_memory_pointer_location, 0x0069, function seletor]
SWAP2 //[value, free_memory_pointer (0x80), 0x0069, new_free_memory_pointer_location, 0x0069, function seletor]
POP //[free_memory_pointer (0x80), 0x0069, new_free_memory_pointer_location, 0x0069, function seletor]
POP //[0x0069, new_free_memory_pointer_location, 0x0069, function seletor]
JUMP //[new_free_memory_pointer_location, 0x0069, function seletor]

//3. Metadata - 
//a. what version?
//b. what optimization?
//c. how this was compiled?
//d. generally used by etherscan verification tool for contract verification
INVALID
LOG2
PUSH5 0x6970667358
INVALID
SLT
KECCAK256
INVALID
INVALID
INVALID
INVALID
PUSH3 0xa8e688
SWAP11
PUSH23 0xfd12d6b9e7364d16aefe1211f170de2ce08e540beb4464
PUSH20 0x6f6c63430008140033