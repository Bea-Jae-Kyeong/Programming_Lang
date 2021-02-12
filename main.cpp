#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
using namespace std;
class Neuron;
typedef vector<Neuron> Layer;
int64_t w[10000], x[10000], sum;
int i = 0;

void BigIntegerTest1(int64_t op1, int64_t op2) {
	cout << "op1+op2=" << op1 + op2 << endl;
	cout << "op1*op2=" << op1 * op2 << endl;
	cout << "op1/op2=" << op1 / op2 << endl;
	cout << "op1%op2=" << op1 % op2 << endl;
}
void BigIntegerTest2(string testfile)
{
	ifstream readf;
	readf.open(testfile);
	while (!readf.eof()) {
		readf >> w[i];
		readf >> x[i];
		sum += w[i] * x[i];
		i++;
	}
	readf.close();
	cout << "result: " << sum << endl;
}
struct Axon
{
	int weight;
	Axon(int weight = 0) : weight(weight) {}
};

class Neuron
{
public:
	unsigned index;
	vector<Axon> vecAxons;
	int64_t output;
	int64_t outputSum;
	Neuron(unsigned index, unsigned nAxons, int state, int layerWeight)
	{

		srand(time(NULL));

		this->index = index;
		this->output = 0;
		int i = 1;
		if (state == 1) {
			for (unsigned a = 0; a < nAxons; a++) {
				this->vecAxons.push_back(Axon(layerWeight));
			}
		}
		else {
			for (unsigned a = 0; a < nAxons; a++) {
				this->vecAxons.push_back(Axon(rand()%9+1));
			}
		}
		
	}
	void feedForward(const Layer &vecPrevLayer)
	{
		this->outputSum = 0;

		for (unsigned n = 0; n < vecPrevLayer.size(); n++) {
			this->outputSum += vecPrevLayer[n].output * vecPrevLayer[n].vecAxons[this->index].weight;
		}
	}

};
class BigIntNeuralNet
{
public:
	vector<Layer> vecLayers;
	int inputNode[10000];
	int layerNum;
	int layerWeight[10000];
	int state;
	int num;
	int64_t output[100];
	vector<unsigned> vecTopology;
	BigIntNeuralNet(string path)
	{
		ifstream readf;
		readf.open(path);
			readf >> state;
			readf >> layerNum;

			for (int i = 0; i < layerNum; i++) {
				readf >> num;
				vecTopology.push_back(num);
			}
			for (int i = 0; i < layerNum-1; i++) {
				readf >> layerWeight[i];
			}
			for (unsigned i = 0; i < vecTopology[0]; i++) {
				readf >> inputNode[i];

			}

			readf.close();
	}
	void InitNN(){
		for (int l = 0; l < layerNum; l++) {
			this->vecLayers.push_back(Layer());

			unsigned nAxons = (l == layerNum - 1) ? 0 : vecTopology[l + 1];
			for (unsigned n = 0; n < vecTopology[l]; n++) {
				this->vecLayers[l].push_back(Neuron(n, nAxons, state, layerWeight[l]));
			
			}

		}
	}

	void ShowResult()
	{
		for (unsigned l = 0; l < this->vecLayers.size(); l++) {

			if (l == 0) {
				cout << "Input : " << vecTopology[l] << endl;
				cout << "  => w : " << layerWeight[l] << "\n";
			}
			else if (l == this->vecLayers.size()-1) {
				cout << "Output : " << vecTopology[l] << endl;
				for (unsigned i = 0; i < vecTopology[l]; i++) {
					cout << "OutNode" << i + 1 << " : " << this->output[i] << endl;
				}
			}
			else {
				Layer &vecLayer = this->vecLayers[l];
					cout << "H" << l << " : " << vecTopology[l] << endl;
					cout << "  => w : " << layerWeight[l] << "\n";
			}
		}
	}

	void setWeight(unsigned layer, unsigned neuron, unsigned axon, int weight)
	{
		this->vecLayers[layer][neuron].vecAxons[axon].weight;
	}

	void feedForward()
	{
		for (unsigned n = 0; n < vecTopology[0]; n++) {
			this->vecLayers[0][n].output=inputNode[n];

		}
		for (unsigned l = 1; l < this->vecLayers.size(); l++) {
			Layer &vecLayer = this->vecLayers[l];
			for (unsigned n = 0; n < vecLayer.size(); n++) {
				vecLayer[n].feedForward(this->vecLayers[l - 1]);
				this->output[n]= vecLayer[n].outputSum;

			}
		}
	}

	vector<int> getOutput()
	{
		vector<int> vecOutputs;

		Layer &vecLayer = this->vecLayers.back();
		for (unsigned n = 0; n < vecLayer.size(); n++) {
			vecOutputs.push_back(vecLayer[n].output);
		}

		return vecOutputs;
	}

};
void TestFFAlgorithm(string path) {

	BigIntNeuralNet nn(path);

	nn.InitNN();
	nn.feedForward();

	nn.ShowResult();


}
void main()
{
	int64_t op1, op2;
	cout << "Big integer calculation" << endl
		<< "Write two big integer numbers : ";
	cin >> op1;
	cout << "one more : ";
	cin >> op2;
	BigIntegerTest1(op1, op2);

	/*Trouble here
	BigIntegerTest2("BigIntegerTestFile.txt");
	TestFFAlgorithm("FFtestFile.txt");
	*/
	system("pause");
}
