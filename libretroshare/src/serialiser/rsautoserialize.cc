#include "serialiser/rsautoserialize.h"

template<> bool processMember<SerializeBuffer, const uint8_t>(SerializeBuffer&op, const uint8_t&value, bool opt)
{
	scae(op.ok, setRawUInt8(&(op.data), op.size, &(op.offset), value) || opt);
	return op.ok;
}
template<> bool processMember<DeserializeBuffer, uint8_t>(DeserializeBuffer&op, uint8_t&value, bool opt)
{
	scae(op.ok, getRawUInt8(&(op.data), op.size, &(op.offset), &value) || opt);
	return op.ok;
}
template<> bool processMember<SerializationSize, const uint8_t>(SerializationSize&op, const uint8_t&, bool)
{
	scae(op.ok, op.size += 1);
	return op.ok;
}

template<> bool processMember<SerializeBuffer, const uint16_t>(SerializeBuffer&op, const uint16_t&value, bool opt)
{
	scae(op.ok, setRawUInt16(&(op.data), op.size, &(op.offset), value) || opt);
	return op.ok;
}
template<> bool processMember<DeserializeBuffer, uint16_t>(DeserializeBuffer&op, uint16_t&value, bool opt)
{
	scae(op.ok, getRawUInt16(&(op.data), op.size, &(op.offset), &value) || opt);
	return op.ok;
}
template<> bool processMember<SerializationSize, const uint16_t>(SerializationSize&op, const uint16_t&, bool)
{
	scae(op.ok, op.size += 2);
	return op.ok;
}
