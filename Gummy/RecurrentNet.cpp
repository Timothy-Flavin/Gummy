#include "RecurrentNet.h"
//riln is recurrent input layer num (layer that is sent to the next datapoint), 
//roln recurrent output layer num (layer that recieves riln's output)
/*Making a recurrent Net requires a list of recurrence relations

*/
RecurrentNet::RecurrentNet(int nl, int*ll, bool so, char* nm, int riln, int roln, int truncNum):DenseNet(nl, ll, so, nm) { //one relation, depricated
	tWeight.construct(layerList[roln],layerList[riln]);
	etWeight.construct(layerList[roln], layerList[riln]);
	etWeightBuffer.construct(layerList[roln], layerList[riln]);
	tWeight.fillRandDouble(-1,1);
	std::cout << "Made time weights matrix:" << std::endl;
	tWeight.print();
	/*activationHistory = new Matrix[truncNum];
	for (int i = 0; i < truncNum; i++) {
		activationHistory[i][i].construct(layerList[roln],1);
	}
	*/ 
}
RecurrentNet::RecurrentNet(int nl, int*ll, bool so, char* nm, recurrentRelation** rltns, int numrltns, int tNum) :DenseNet(nl, ll, so, nm) { //multiple relations
	relations = rltns;
	numRelations = numrltns;
	truncNum = tNum;
																																			 //relations are a ** so each one points to a pointer to a matrix so that they can be easily sorted
	sortRelations(relations, numRelations);
	//constructing matrices for relations and allocating memory for activation history
	activationHistory = new Matrix*[numRelations];
	for (int i = 0; i < numRelations; i++) {
		relations[i]->weightMatrix.construct(layerList[relations[i]->outputLayer], layerList[relations[i]->inputLayer]);
		relations[i]->errorMatrix.construct(layerList[relations[i]->outputLayer], layerList[relations[i]->inputLayer]);
		relations[i]->errorMatrixBuffer.construct(layerList[relations[i]->outputLayer], layerList[relations[i]->inputLayer]);
		relations[i]->weightMatrix.fillRandDouble(-1, 1);
		std::cout << "Made time weights matrix: "<<i<< std::endl;
		relations[i]->weightMatrix.print();
		activationHistory[i] = new Matrix[truncNum];
		for (int j = 0; j < truncNum; j++) {
			activationHistory[i][j].construct(layerList[relations[i]->outputLayer], 1);
		}
	}

	std::cout << "FINISHED CONSTRUCTING RECURRENT NETWORK";
}

void RecurrentNet::sortRelations(recurrentRelation** relations, int numRelations) {
	recurrentRelation* tempRelation = nullptr;
	for (int i = 0; i < numRelations-1; i++) {
		for (int j = 0; j < numRelations - i - 1; j++) {
			if (relations[j]->outputLayer < relations[j + 1]->outputLayer){
				tempRelation = relations[j];
				relations[j] = relations[j + 1];
				relations[j+1] = tempRelation;
			}
		}
	}
}

void RecurrentNet::printRelations(recurrentRelation** relations, int numRelations) {
	for (int i = 0; i < numRelations; i++) {
		std::cout << "Relation " << i << std::endl << "Input Layer: " << relations[i]->inputLayer << std::endl;
		std::cout << "Output Layer: " << relations[i]->outputLayer << std::endl;
		std::cout << "Time Distance: " << relations[i]->timeDistance << std::endl;
		//Matrix weightMatrix, errorMatrix, errorMatrixBuffer;
		std::cout << "Weight Matrix: ";
		relations[i]->weightMatrix.print(); 
		std::cout<< std::endl;
		std::cout << "Error Matrix: ";
		relations[i]->errorMatrix.print();
		std::cout << std::endl;
		std::cout << "Error Matrix Buffer: ";
		relations[i]->errorMatrixBuffer.print();
		std::cout << std::endl;
	}
}

RecurrentNet::RecurrentNet(csv* file) : DenseNet(){
	std::cout << "making new net from file";
	name = file->name;
	std::cout << "name: " << name << std::endl;
	numLayers = file->numData[0][0];
	std::cout << "num Layers: " << numLayers << std::endl;
	sigmoidOutput = file->numData[0][1] == 1 ? true : false;
	std::cout << "sigmoid: " << sigmoidOutput << std::endl;
	layerList = new int[numLayers];
	for (int i = 0; i < numLayers; i++) {
		layerList[i] = file->numData[0][i + 2];
		std::cout << "layerList " << i << ": " << layerList[i] << std::endl;
	}
	activations = new Matrix[numLayers];
	weights = new Matrix[numLayers - 1];
	bias = new Matrix[numLayers - 1];
	eWeights = new Matrix[numLayers - 1];
	eBias = new Matrix[numLayers - 1];
	eBiasBuffer = new Matrix[numLayers - 1];
	eActivation = new Matrix[numLayers];
	//std::cout << "weights" << std::endl;
	int currentRow = 1;
	for (int i = 0; i < numLayers - 1; i++) {
		weights[i].construct(layerList[i + 1], layerList[i]);
		for (int r = 0; r < weights[i].getM(); r++) {
			for (int c = 0; c < weights[i].getN(); c++) {
				weights[i].set(r, c, file->numData[currentRow][c]);
				//std::cout<<weights[i].get(r,c)<<", ";
			}
			currentRow++;
			//std::cout<<std::endl;
		}
		eWeights[i].construct(layerList[i + 1], layerList[i]);
	}
	//std::cout << "bias" << std::endl;
	for (int i = 0; i < numLayers - 1; i++) {
		bias[i].construct(layerList[i + 1], 1);
		for (int r = 0; r < bias[i].getM(); r++) {
			bias[i].set(r, 0, file->numData[currentRow][r]);
			//std::cout << bias[i].get(r, 0) << ", ";
		}
		//std::cout << std::endl;
		currentRow++;
		eBias[i].construct(layerList[i + 1], 1);
		eBiasBuffer[i].construct(layerList[i + 1], 1);
	}
	for (int i = 0; i < numLayers; i++) {
		activations[i].construct(layerList[i], 1);
		eActivation[i].construct(layerList[i], 1);
	}
}
Matrix* RecurrentNet::feedForward(Matrix* inputs) {
	return inputs;
}
void RecurrentNet::backProp(Matrix* A, double stepSize){
}
void RecurrentNet::updateWeights(double stepSize, int batchSize){
}
void RecurrentNet::print(){
	DenseNet::print();
	printRelations(relations, numRelations);
}
void RecurrentNet::printGradient(){
}
void RecurrentNet::save(){
}