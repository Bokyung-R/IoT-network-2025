# IoT-network-2025
IoT 개발자 네트워크 리포지토리

## 2일차

- inet_addr : .으로 연결되어있는 문자열을 정수형태로 만드는데 특화 되어있는 함수
    - ex) 
    
- htonl() : 호스트를 네트워크로 변환
    - ex) htonl(0x100a8c0) -> 0xc0a80001 == 호스트 : 리틀인디언, 네트워크 : 빅인디엄

- inet_ntoa() : 정수 ip를 문자열 주소로 변환
    - ex) inet_ntoa(0xc0a80001) -> 1.0.168.192

```c
// IPv4 관련 구조체
netinet/in.h

typedef unsigned short int sa_family_t
typedef  uint32_t  in_addr_t;
typedet  uint16_t  in_port_t;

struct  in_addr{
    in_addr_t  s_addr;             // 32bit IPv4 인터넷 주소, in_addr_t 는 uint32_t 
};

struct  sockaddr_in{			   //IPv4 정보를 취급하기위한 구조체
    __SOCKADDR_COMMON(sin_);
    in_port_t sin_port;            // 16비트 TCP/UDP PORT 번호
    struct in_addr sin_addr;       // 32비트 IP 주소 - in_addr 구조체를 사용

    unsigned char sin_zero[sizeof(struct sockaddr) – __SOCKADDDR_COMMON_SIZE  - (in_port_t) – (struct in_addr)];   // 16 – 2 - 2 - 4 = 8byte
  };	

struct sockaddr { sa_family_t sin_family;  char sa_data[14]; }      // size : 16byte
```

- `(sockaddr_int)addr_inet.sin_addr.s_addr` 
    - `sockaddr_in` 구조체 안에는 `in_addr` 타입의 `sin_addr` 변수가 있음.
    - `in_addr` 구조체는 `in_addr_t`타입의 `s_addr` 변수를 가지고 있으며, 4바이트 IPv4 주소를 저장.
    - `sin_addr` 자체는 구조체이므로 실제 IP 값에 접근하려면 `sin_addr.s_addr`처럼 멤버를 통해 접근해야 함.

- `sockaddr_in.sin_zero` 설명
    - 구조체의 크기를 `struct sockaddr` (16바이트)와 일치시키기 위한 **패딩** 필드.
    - 실제 네트워크 프로그래밍에서는 사용되지 않으며, 일반적으로 `0`으로 초기화됨.


AF_INET : 주소체계
PF_INET : 프로토콜패밀리
-> 둘다 IPv4를 나타냄

- 서버 프로그램 -> 2개의 소켓을 생성함
    1. `서버 소켓` : 서버파일 스크립트 : 클라이언트 요청을 받아들이는 소켓  
        (연결 요청 들어옴 -> 대기큐에 대기 -> aceept 함수 -> 새로운 소켓(클라이언트 소켓) 탄생)
    2. `클라이언트 소켓` : 클라이언트 파일 스크립트 : 연결이 된 다음 데이터를 주고받는 소켓 : 해당 클라이언트와 서버가 통신하는 소켓탕생

