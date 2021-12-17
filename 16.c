
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>

#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))
#define Minimum(a, b) ((a) < (b) ? (a) : (b))
#define Maximum(a, b) ((a) > (b) ? (a) : (b))

typedef uint32_t u32;
typedef uint64_t u64;

typedef struct {
    int count;
    int at;
    unsigned char *data;
} bit_reader;

u32 get_byte(bit_reader *b, int index) {
    if (index < b->count) return b->data[index];
    return 0;
}

u32 bit_eat(bit_reader *b, u32 count) {
    u32 byte_index = b->at / 8;
    u32 bit_index = b->at % 8;
    
    u32 result = 0;
    result |= get_byte(b, byte_index+0) << 24;
    result |= get_byte(b, byte_index+1) << 16;
    result |= get_byte(b, byte_index+2) << 8;
    result |= get_byte(b, byte_index+3) << 0;
    
    result <<= bit_index;
    result >>= (32U - count);
    
    b->at += count;
    
    return result;
}

unsigned char hex_digit_to_nibble(char c) {
    unsigned char result = 0;
    if (c >= '0' && c <= '9') {
        result = c - '0';
    } else if (c >= 'A' && c <= 'F') {
        result = c - 'A' + 10;
    }
    return result;
}

enum {
    SUM = 0,
    PRODUCT = 1,
    MINIMUM = 2,
    MAXIMUM = 3,
    LITERAL = 4,
    GREATER_THAN = 5,
    LESS_THAN = 6,
    EQUAL_TO = 7,
    SUM_VERSION,
};

u64 parse_packets(bit_reader *b, int in_bits, int n, u32 op) {
    int at = b->at;
    
    u64 result = 0;
    switch (op) {
        case PRODUCT: result = 1; break;
        case MINIMUM: result = UINT64_MAX; break;
    }
    
    u64 last_value = 0;
    
    int packet_count = 0;
    for (;;) {
        packet_count++;
        
        u32 version = bit_eat(b, 3);
        u32 type = bit_eat(b, 3);
        
        if (op == SUM_VERSION) result += version;
        
        u64 value = 0;
        if (type == LITERAL) {
            u32 should_continue = 1;
            while (should_continue) {
                should_continue = bit_eat(b, 1);
                u64 v = (u64)bit_eat(b, 4);
                value <<= 4;
                value |= v;
            }
        } else {
            u32 length_type_id = bit_eat(b, 1);
            int in_bits = 0;
            int n = 0;
            if (length_type_id) {
                n = bit_eat(b, 11);
            } else {
                in_bits = 1;
                n = bit_eat(b, 15);
            }
            
            value = parse_packets(b, in_bits, n, op == SUM_VERSION ? SUM_VERSION : type);
        }
        
        switch (op) {
            case SUM_VERSION:  if (type != LITERAL) result += value;                break;
            case SUM:          result += value;                                     break;
            case PRODUCT:      result *= value;                                     break;
            case MINIMUM:      result = Minimum(result, value);                     break;
            case MAXIMUM:      result = Maximum(result, value);                     break;
            case GREATER_THAN: if (packet_count == 2) result = last_value > value;  break;
            case LESS_THAN:    if (packet_count == 2) result = last_value < value;  break;
            case EQUAL_TO:     if (packet_count == 2) result = last_value == value; break;
        }
        
        last_value = value;
        
        if (in_bits) {
            if (b->at - at >= n) break;
        } else {
            if (packet_count >= n) break;
        }
    }
    
    // sanity check
    switch (op) {
        case GREATER_THAN:
        case LESS_THAN:
        case EQUAL_TO: assert(packet_count == 2);
    }
    
    return result;
}

int main(void) {
    unsigned char byte_input[2048] = {0};
    int count = 0;
    
    char hex_input[4096] = {0};
    fgets(hex_input, sizeof(hex_input), stdin);
    
    for (char *c = hex_input; isxdigit(*c); c += 2) {
        unsigned char n = 
            hex_digit_to_nibble(c[0]) << 4 |
            hex_digit_to_nibble(c[1]);
        
        byte_input[count++] = n;
    }
    
    bit_reader a = {count, 0, byte_input};
    bit_reader b = a;
    
    u64 result = parse_packets(&a, 0, 1, SUM_VERSION);
    printf("%llu\n", result);
    result = parse_packets(&b, 0, 1, SUM);
    printf("%llu\n", result);
}