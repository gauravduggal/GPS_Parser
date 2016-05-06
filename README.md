compile with gcc main.c -o a
run with ./a

Working:

1). This parser reads data from GPS.txt and fills a ring buffer. On finding a valid nmea 0183 string it returns that from the ring buffer. It checks for the checksum so garbage on the rs232 line / from file does not affect the data.

2). Another function decodes data from the GPGGA string.

3). Can be easily modified to work with RS232 directly.
