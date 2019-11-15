#include "RecurrentNet.h"
//riln is recurrent input layer num (layer that is sent to the next datapoint), 
//roln recurrent output layer num (layer that recieves riln's output)
/*Making a recurrent Net requires a list of recurrence relations

*/
RecurrentNet::RecurrentNet(int nl, int*ll, bool so, char* nm, int riln, int roln, int truncNum):DenseNet(nl, ll, so, nm) { //one relation, depricated
	//jerry = 5;
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
RecurrentNet::RecurrentNet(int nl, int*ll, bool so, char* nm, recurrentRelation** rltns, int numrltns, int tNum):DenseNet(nl, ll, so, nm) { //multiple relations
	relations = rltns;
	numRelations = numrltns;
	truncNum = tNum;
																																			 //relations are a ** so each one points to a pointer to a matrix so that they can be easily sorted
	sortRelations();//relations, numRelations);
	//constructing matrices for relations and allocating memory for activation history
	activationHistory = new Matrix*[numRelations];
	for (int i = 0; i < numRelations; i++) {
		relations[i]->weightMatrix.construct(layerList[relations[i]->outputLayer], layerList[relations[i]->inputLayer]);
		relations[i]->errorMatrix.construct(layerList[relations[i]->outputLayer], layerList[relations[i]->inputLayer]);
		relations[i]->errorMatrixBuffer.construct(layerList[relations[i]->outputLayer], layerList[relations[i]->inputLayer]);
		relations[i]->weightMatrix.fillRandDouble(-1, 1);

		//construct bias matrices
		std::cout << "Made time weights matrix: "<<i<< std::endl;
		//relations[i]->weightMatrix.print();
		relations[i]->activationHistory = new Matrix[truncNum];
		for (int j = 0; j < truncNum; j++) {
			relations[i]->activationHistory[j].construct(layerList[relations[i]->outputLayer], 1);
		}
	}

	std::cout << "FINISHED CONSTRUCTING RECURRENT NETWORK";
}

void RecurrentNet::sortRelations(){//recurrentRelation** relations, int numRelations) {
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

void RecurrentNet::printRelations(){//recurrentRelation** relations, int numRelations) {
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
	/*
	int numIns = inputs->getM();
	for (int i = 0; i < numIns; i++) {
		activations[0].set(i, 0, inputs->get(i, 0));
	}
	for (int i = 0; i < numLayers - 1; i++) {
		weights[i].multiply(&activations[i], &activations[i + 1]);
		activations[i+1].add(&bias[i], &activations[i+1]);
		if (!(i == numLayers - 2) || sigmoidOutput) {
			gate(&activations[i + 1]);
		}
	}
	return &activations[numLayers-1];
	*/
	int numIns = inputs->getM();
	for (int i = 0; i < numIns; i++) {
		activations[0].set(i, 0, inputs->get(i, 0));
	}
	for (int i = 0; i < numLayers - 1; i++) {
		weights[i].multiply(&activations[i], &activations[i + 1]);
		activations[i + 1].add(&bias[i], &activations[i + 1]);
		for (int j = numRelations - 1; j >= 0; j--) {
			//if a relation has this layer as an output and this timestep is one where it matters, add its hidden layer to this one
			if (relations[i]->outputLayer == i && relations[i]->timeDistance <= currentTime && currentTime % relations[i]->timeDistance) {
				/*the relation outputs to this layer, there has eben enough time for a relation to exist, and this time step is one where this relation plays a role
					we need to multiply the most recent activation saved in this relations activations[] by the relation's weights and then add it's bias
					then add that to this activations layer
				*/
				relations[j]->weightMatrix.multiply(&relations[j]->activationHistory[tailOfTruncatedArray], &relations[j]->mostRecentActivation);
				//this line should multiply the weight matrix by the activation history's most recent activation and store the result into most recent activation
				relations[j]->mostRecentActivation.add(&relations[j]->biasMatrix, &relations[j]->mostRecentActivation);
			}
			if (relations[i]->outputLayer > i) {
				j = -1; //exit if we have passed up the relations with output layers less than the current layer
			}
		}
		if (!(i == numLayers - 2) || sigmoidOutput) {
			gate(&activations[i + 1]);
		}
	}
	return &activations[numLayers - 1];
	return inputs;
}
void RecurrentNet::backProp(Matrix* A, double stepSize){
}
void RecurrentNet::updateWeights(double stepSize, int batchSize){
}
void RecurrentNet::print(){
	print();
	printRelations();
}
void RecurrentNet::printGradient(){
}
void RecurrentNet::save(){
}