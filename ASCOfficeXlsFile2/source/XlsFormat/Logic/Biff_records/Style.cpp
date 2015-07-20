
#include "Style.h"

#include <simple_xml_writer.h>

namespace XLS
{;

Style::Style()
{
}


Style::~Style()
{
}


BaseObjectPtr Style::clone()
{
	return BaseObjectPtr(new Style(*this));
}


void Style::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBITS(flags, 0, 11, ixfe);
	SETBIT(flags, 15, fBuiltIn);
	record << flags;
	if(fBuiltIn)
	{
		record << builtInData;
	}
	else
	{
		record << user;
	}
}


void Style::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	ixfe = GETBITS(flags, 0, 11);
	fBuiltIn = GETBIT(flags, 15);
	if(fBuiltIn)
	{
		record >> builtInData;
	}
	else
	{
		record >> user;
	}
}

int Style::serialize(std::wostream & stream)
{
	return 0;
}

} // namespace XLS

