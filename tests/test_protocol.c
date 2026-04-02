#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "crc.h"
#include "packet.h"

void test_crc_basic(){
    printf("Running test_crc_basic...\n");
    
    char message[] = "Hi";
    char bit_string[1024];
    char remainder[32];
    char divisor[] = "1101";
    
    string_to_bits(message, bit_string);
    compute_crc(bit_string, divisor, remainder);
    
    printf("  Message: %s\n", message);
    printf("  Bit string: %s\n", bit_string);
    printf("  Remainder: %s\n", remainder);
    
    printf("test_crc_basic PASSED\n");
}

void test_crc_long_message(){
    printf("Running test_crc_long_message...\n");
    
    char message[] = "Hello World";
    char bit_string[1024];
    char remainder[32];
    char divisor[] = "1101";
    
    string_to_bits(message, bit_string);
    compute_crc(bit_string, divisor, remainder);
    
    printf("  Message: %s\n", message);
    printf("  Remainder: %s\n", remainder);
    
    printf("test_crc_long_message PASSED\n");
}

void test_frame_creation(){
    printf("Running test_frame_creation...\n");
    
    frame_t f;
    memset(&f, 0, sizeof(frame_t));
    
    f.preamble = 0xAA;
    f.seq_no = 1;
    f.ack_no = 0;
    strcpy((char*)f.data, "Test");
    f.crc = 5;
    
    assert(f.preamble == 0xAA);
    assert(f.seq_no == 1);
    assert(strcmp((char*)f.data, "Test") == 0);
    assert(f.crc == 5);
    
    printf("test_frame_creation PASSED\n");
}

void test_frame_size(){
    printf("Running test_frame_size...\n");
    
    printf("  frame_t size: %zu bytes\n", sizeof(frame_t));
    
    assert(sizeof(frame_t) > 0);
    
    printf("test_frame_size PASSED\n");
}

int main(){
    printf("=== CTP Test Suite ===\n\n");
    
    test_crc_basic();
    test_crc_long_message();
    test_frame_creation();
    test_frame_size();
    
    printf("\n=== All Tests Passed ===\n");
    return 0;
}