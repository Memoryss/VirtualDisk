#include "VirtualDisk.h"

#include <gtest/gtest.h>

class VirtualDiskUnit : public ::testing::Test
{
public:
	virtual void SetUp();
	virtual void TearDown();

protected:
	VirtualDisk _disk;
};