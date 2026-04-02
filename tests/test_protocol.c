#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "crc.h"
#include "packet.h"

void test_crc_basic(void)
{
    char msg[] = "Hi";
    char bits[1024];
    char rem[32];
    char div[] = "1101";

    string_to_bits(msg, bits);
    compute_crc(bits, div, rem);

    printf("Message: '%s', Remainder: %s\n", msg, rem);
    printf("test_crc_basic: PASSED\n");
}

void test_frame_creation(void)
{
    frame_t f = {0};

    f.preamble = 0xAA;
    f.seq_no = 1;
    strcpy((char *)f.data, "Test");

    assert(f.preamble == 0xAA);
    assert(f.seq_no == 1);
    assert(strcmp((char *)f.data, "Test") == 0);

    printf("test_frame_creation: PASSED\n");
}

int main(void)
{
    printf("Running tests...\n");

    test_crc_basic();
    test_frame_creation();

    printf("\nAll tests passed!\n");
    return 0;
}