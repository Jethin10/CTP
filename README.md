# **CTP — Custom Transport Protocol**

![Alt Text](logo.png)

---

**CTP**, is a frame-based **custom transport protocol** implemented in pure C. This project simulates **reliable serial communication** over virtual or real serial ports, including frame creation, CRC error detection, and ACK/NACK based retransmission.

---

## 🌟 Features

| Protocol Feature              | CAP Implementation                               |
|-------------------------------|-------------------------------------------------|
| Frame Structure               | Each frame has preamble, sequence number, payload, and CRC |
| CRC Error Detection           | Detects transmission errors using binary CRC   |
| ACK/NACK Reliability          | Receiver sends ACK/NACK; sender retransmits if needed |
| Virtual Serial Communication  | Uses `socat` to simulate connected serial ports |
| Termios Integration           | Configures serial ports, baud rate, character size, and blocking mode |
| Simple Data Transfer          | Send arbitrary payloads reliably over serial   |
| Partial Read Handling         | State machine approach handles fragmented reads |

## Frame Format

```
+--------+----------+--------+--------+------+
| Preamble| Seq No  | Ack No |  Data  | CRC  |
|  1 byte | 2 bytes | 2 bytes| 512 max| 1byte|
+--------+----------+--------+--------+------+
```

## Architecture

![Alt Text](arch.png)

## Getting Started

### Requirements
- GCC / C compiler  
- Unix-based OS (Linux, macOS)  
- `socat` (for virtual serial port testing)  
- `make`  

### Build and Run

```bash
# Terminal 1 - Setup virtual serial ports
socat -d -d pty,raw,echo=0,link=/tmp/sender pty,raw,echo=0,link=/tmp/receiver

# Terminal 2 - Build and run receiver
make
make run-receiver

# Terminal 3 - Run sender
make run-sender
```

## Testing

```bash
# Run unit tests
gcc -I src/include tests/test_protocol.c src/src/crc.c src/src/packet.c -o test_runner
./test_runner
```

## Contributing

We ❤️ contributors!

To contribute:

1. Fork the repository.
2. Clone it locally:
   ```bash
   git clone [your-fork-url]
   cd ctp
   git checkout -b feature/my-improvement
   ```
3. Make your changes and submit a pull request.

## Related

[CTP Blog](https://meerthika.medium.com/building-a-frame-based-custom-transport-protocol-in-c-2c9fbd404428)

## License

MIT - @Meerthika
