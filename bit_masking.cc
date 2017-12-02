#include <vector>
#include <iostream>

#include <cstdint>

using namespace std;

// fixme: maybe do templ.
class DataLeafNode {
public:
	typedef int Index;

	// 8 bit
	typedef uint8_t Cell;  // возможно лучше сделать побольше
	static const int DIM = 8;
	static const int Log2Dim = 3; // 8 - dim

	DataLeafNode();

private:
	vector<Cell> mask;
	int max_offset;

	// fixme: как хранить значение, вне класса?
	// fixme: как хранить если нужно несколько листов, по разным измерениям?
};

class WholeData2LayerDeep {
public:
	// вержний уровень и один ниже
};

DataLeafNode::DataLeafNode() {
	vector<uint8_t>(DIM * DIM).swap(mask);
	max_offset = DIM * DIM * DIM;
}

int main() {
	// "OpenVDB: An Open Source Data Structure and Toolkit for High-Resolution Volumes"
	// fixme: how to save?
	// fixme: how to iterate
	// fixme: не ясен порадок укладки
	DataLeafNode::Index offset = 0;
	{
		int x = 2;
		int y = 3;
		int z = 1;

		const DataLeafNode::Index offsetX = (x & (DataLeafNode::DIM - 1u))
				<< 2 * DataLeafNode::Log2Dim;
		const DataLeafNode::Index offsetXY = offsetX
				+ ((y & (DataLeafNode::DIM - 1u)) << DataLeafNode::Log2Dim);
		offset = offsetXY + (z & (DataLeafNode::DIM - 1u));

	}

	cout << "offset:" << offset << endl;

	{
		//xPos = (ijk[0] & (DIM - 1u)) << (2 * LOG2DIM);
		//yPos = xPos + ((ijk[1] & (DIM - 1u)) << LOG2DIM);
		//pos = yPos + (ijk[2] & (DIM - 1u));
	}
	// 8 - z
	// 8x8 - xy
	//uint32_t mask = 0x10;

	// 32 bit
	int sLog2X = DataLeafNode::Log2Dim;	//4;
	int sLog2Y = DataLeafNode::Log2Dim;	//4;
	int sLog2Z = DataLeafNode::Log2Dim;	//2;

	//int offset = 11;
	int x = offset >> sLog2Y + sLog2Z;
	int n = offset & ((1 << sLog2Y + sLog2Z) - 1);
	int y = n >> sLog2Z;
	int z = n & (1 << sLog2Z) - 1;

	cout << x << endl;
	cout << y << endl;
	cout << z << endl;

	return 0;
}
