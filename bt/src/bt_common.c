#include "bt_common.h"


uint16_t bt_le_read_16(const uint8_t * buffer, int pos)
{
    return (uint16_t)(((uint16_t) buffer[pos]) | (((uint16_t)buffer[(pos)+1]) << 8));
}
uint32_t bt_le_read_24(const uint8_t * buffer, int pos)
{
    return ((uint32_t) buffer[pos]) | (((uint32_t)buffer[(pos)+1]) << 8) | (((uint32_t)buffer[(pos)+2]) << 16);
}
uint32_t bt_le_read_32(const uint8_t * buffer, int pos)
{
    return ((uint32_t) buffer[pos]) | (((uint32_t)buffer[(pos)+1]) << 8) | (((uint32_t)buffer[(pos)+2]) << 16) | (((uint32_t) buffer[(pos)+3]) << 24);
}


void bt_le_store_16(uint8_t *buffer, uint16_t pos, uint16_t value)
{
    buffer[pos++] = (uint8_t)value;
    buffer[pos++] = (uint8_t)(value >> 8);
}

void bt_le_store_24(uint8_t *buffer, uint16_t pos, uint32_t value)
{
    buffer[pos++] = (uint8_t)(value);
    buffer[pos++] = (uint8_t)(value >> 8);
    buffer[pos++] = (uint8_t)(value >> 16);
}

void bt_le_store_32(uint8_t *buffer, uint16_t pos, uint32_t value)
{
    buffer[pos++] = (uint8_t)(value);
    buffer[pos++] = (uint8_t)(value >> 8);
    buffer[pos++] = (uint8_t)(value >> 16);
    buffer[pos++] = (uint8_t)(value >> 24);
}