// SPDX-License-Identifier: UNLICENSED
pragma solidity ^0.8.13;

contract Middleman {
  address public target = 0xbAF0fb20604d822f8d4E1A35250669Ca5b8D8038;

  function attack(uint256 _damage) external {
    (bool success, bytes memory result) = target.call(abi.encodeWithSignature("attack(uint256)", _damage));
    require(success, string(result));
  }
}