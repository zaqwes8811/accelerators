#include <vector>
#include <iostream>

#include <cstdint>
#include <cassert>

using namespace std;

// tricks
// https://stackoverflow.com/questions/1533131/what-useful-bitwise-operator-code-tricks-should-a-developer-know-about
// https://www.topcoder.com/community/data-science/data-science-tutorials/a-bit-of-fun-fun-with-bits/

// fixme: maybe do templ.
// https://stackoverflow.com/questions/14707803/line-size-of-l1-and-l2-caches - i7 64 байта!!
template<typename T>
class DataLeafNode {
public:
	typedef int Index;

	struct vec3 {
		vec3() {
			x = y = z = 0;
		}
		int x, y, z;
	};

	// 8 bit
	typedef uint8_t Cell;  // возможно лучше сделать побольше
	static const int DIM = 8;
	static const int Log2Dim = 3; // 8 - dim

	DataLeafNode() {
		vector<Cell>(DIM * DIM, 0).swap(mask_bit_buffer); // set to zero
		vector<T>(DIM * DIM * DIM, T(0)).swap(data_buffer);  // 3d
		max_offset = DIM * DIM * DIM;
	}

	void traverse() {
		const int cells_count = DIM * DIM;
		vector<Index> offsets;
		offsets.reserve(cells_count * 2);  // ни к чему резервить много

		// https://graphics.stanford.edu/~seander/bithacks.html !!!
		// fixme: получить список оффсетов с непустыми битами
		// https://stackoverflow.com/questions/20713017/what-is-the-fastest-way-to-return-the-positions-of-all-set-bits-in-a-64-bit-inte
		int whole_mask_ptr = 0;
		for (const auto l : mask_bit_buffer) {
			// не хочется сдвигать
			const auto l_tmp = l;
			for (auto i = 0; i < DIM; ++i) {
				if (l_tmp & (0x01 << i)) {
					offsets.push_back(whole_mask_ptr);
				}

//				l_tmp >>= 1;
				++whole_mask_ptr;
			}
		}

		// можно выгрузить/скомпрессить данные
	}

	void compress() {
		// возможно это типа разово можно сделать
	}

	void generateMaskByConstraints() {
		// если ищем в заданном квадрате
	}

	Index toOffset(int x, int y, int z) {
		assert(x < 8 && y < 8 && z < 8);
		assert(x >= 0 && y >= 0 && z >= 0);

		const Index offsetX = (x & (DIM - 1u)) << 2 * Log2Dim;
		const Index offsetXY = offsetX + ((y & (DIM - 1u)) << Log2Dim);
		const Index offset = offsetXY + (z & (DIM - 1u));

		return offset;
	}

	void setActive(int x, int y, int z, const T& val) {
		auto offset = toOffset(x, y, z);

		// store bit
		int index = offset / DIM;  // + 1u;
		int shift = offset - index * DIM;
		mask_bit_buffer[index] |= 0x01 << shift;  // сдвиг может быть долгим?

		// non compressed way, ready to random access
		data_buffer[offset] = val;
	}

	void fromOffset(Index offset) {
		int sLog2X = Log2Dim;
		int sLog2Y = Log2Dim;
		int sLog2Z = Log2Dim;

		int x = offset >> sLog2Y + sLog2Z;
		int n = offset & ((1 << sLog2Y + sLog2Z) - 1);
		int y = n >> sLog2Z;
		int z = n & (1 << sLog2Z) - 1;

		cout << x << endl;
		cout << y << endl;
		cout << z << endl;
	}

private:
	vector<Cell> mask_bit_buffer;
	vector<T> data_buffer;
	vector<T> compressed_data_buffer;  // not used now
	int max_offset;

	// как с рандомным доступом быть?
	// fixme: как хранить значение, вне класса?
	// похоже внутри класса, но как найти соответствие
	// fixme: как хранить если нужно несколько листов, по разным измерениям?
};

//========================================================================

class WholeData2LayerDeep {
public:
	// вержний уровень и один ниже
	WholeData2LayerDeep();

	static const int sz = 2;

	void setActive(int x, int y, int z);

	vector<DataLeafNode<int> > leafs;  // 2x2x2 - 8 items
};

WholeData2LayerDeep::WholeData2LayerDeep() {
	vector<DataLeafNode<int>>(sz * sz * sz).swap(leafs);
}

void WholeData2LayerDeep::setActive(int x, int y, int z) {

}

//========================================================================

int main() {
	// "OpenVDB: An Open Source Data Structure and Toolkit for High-Resolution Volumes"
	//
	// caches
	// https://www.edn.com/design/systems-design/4399725/Memory-Hierarchy-Design---Part-6--The-Intel-Core-i7
	// https://www.edn.com/design/systems-design/4397051/Memory-Hierarchy-Design-part-1

	//
	// fixme: how to save?
	// fixme: how to iterate
	int x = 2;
	int y = 3;
	int z = 1;

	if (0) {
		// fixme: не ясен порадок укладки
		for (auto y = 0; y < DataLeafNode<int>::DIM; ++y) {
			auto offset = DataLeafNode<int>().toOffset(x, y, z);
			cout << "offset:" << offset << endl;
			DataLeafNode<int>().fromOffset(offset);
		}
	}

	cout << endl;
	x = 0;
	y = 0;
	z = 7;
	auto ln = DataLeafNode<int>();
	ln.setActive(x, y, z, 0);
	ln.traverse();

	return 0;
}
