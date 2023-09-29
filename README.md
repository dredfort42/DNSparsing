# DNSparsing - DNS message parser
## Job interview testing task
Build a DNS message parser.

Build a program that can take a DNS message in binary format encoded in hex strings, parse it into appropriate DNS message and print it out in human friendly string representation.

You code should support at least the A, AAAA and CNAME record types.

***Input:***  
> a01d81800001000100000000076578616d706c6503636f6d0000010001c00c0001000100001bbc00045db8d822

***Expected Output:***
> ;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 40989  
> ;; flags: qr rd ra; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 0
> 
> ;; QUESTION SECTION:  
> ;example.com.		IN	A
> 
> ;; ANSWER SECTION:  
> example.com.		7100	IN	A	93.184.216.34

### Build
```bash
g++ main.cpp -o main
```

### Run
```bash
./main
# input hex DNS message and press Enter
```