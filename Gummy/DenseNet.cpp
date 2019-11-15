#include"DenseNet.h"
/*Creates a dense neural net with a number of layers "nl" and a pointer to an 
  array or layer sizes "ll". bool so decides whether the output is sigmoid (0-1)
  or raw (Any double). nm is the name which the neural net will use when saving
  itself to a file
*/
DenseNet::DenseNet(int nl, int*ll, bool so, char* nm):NeuralNetwork() {
	sigmoidOutput = so;
	if (nl < 2)
		std::cout << "Need at least an input layer and output layer";
	numLayers = nl;
	layerList = ll;
	name = nm;
	//matrix list of node values
	activations = new Matrix[numLayers];
	//weights matrix list between nodes
	weights = new Matrix[numLayers - 1];
	//matrix list of bias to be added
	bias = new Matrix[numLayers - 1];
	eWeights = new Matrix[numLayers - 1];
	eBias = new Matrix[numLayers - 1];
	eBiasBuffer = new Matrix[numLayers - 1];
	eActivation = new Matrix[numLayers]; //can cut out first row if you want
	for (int i = 0; i < numLayers-1; i++) {
		weights[i].construct(layerList[i+1], layerList[i]);
		weights[i].fillRandDouble(-1, 1);
		eWeights[i].construct(layerList[i + 1], layerList[i]);
	}
	for (int i = 0; i < numLayers-1; i++) {
		bias[i].construct(layerList[i+1], 1);
		bias[i].fillRandDouble(-1, 1);
		eBias[i].construct(layerList[i + 1], 1);
		eBiasBuffer[i].construct(layerList[i + 1], 1);
	}
	for (int i = 0; i < numLayers; i++) {
		activations[i].construct(layerList[i], 1);
		eActivation[i].construct(layerList[i], 1);
	}
}
//given a csv file loads a dense net into ram for use. 
DenseNet::DenseNet(csv* file){
	std::cout<<"making new net from file";
	name = file->name;
	std::cout<<"name: "<<name<<std::endl;
	numLayers = file->numData[0][0];
	std::cout<<"num Layers: "<<numLayers<<std::endl;
	sigmoidOutput = file->numData[0][1] == 1?true:false;
	std::cout<<"sigmoid: "<<sigmoidOutput<<std::endl;
	layerList = new int[numLayers];
	for(int i = 0; i < numLayers; i++){
		layerList[i]=file->numData[0][i+2];
		std::cout<<"layerList "<<i<<": "<<layerList[i]<<std::endl;
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
	for (int i = 0; i < numLayers-1; i++) {
		weights[i].construct(layerList[i+1], layerList[i]);
		for(int r = 0; r < weights[i].getM(); r++){
			for(int c = 0; c < weights[i].getN();c++){
				weights[i].set(r,c,file->numData[currentRow][c]);
				//std::cout<<weights[i].get(r,c)<<", ";
			}
			currentRow++;
			//std::cout<<std::endl;
		}
		eWeights[i].construct(layerList[i + 1], layerList[i]);
	}
	//std::cout << "bias" << std::endl;
	for (int i = 0; i < numLayers-1; i++) {
		bias[i].construct(layerList[i+1], 1);
		for(int r = 0; r < bias[i].getM(); r++){
			bias[i].set(r,0,file->numData[currentRow][r]);
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
/*
Forward pass algorithm for dense neural networks. Uses sigmoid gates
*/
DenseNet::DenseNet() {

}
Matrix* DenseNet::feedForward(Matrix* inputs) {
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
}
// make a back prop that does stocastic gradient descent with samples of data rather than 1 as an option
void DenseNet::backProp(Matrix* A, double stepSize) {
	//error*S()*(1-S())
	/*
	1: set last row of eActivations through calcError
	Loop for num layers{
	2: set ebias with inversegate(activations[i]) * eActivation[i]
	3: set each w[i] by corrisponding ebias[i]*activations[i] (row before the weights row)
	4: set next eActivations via transpose multiply weights[i]*activations[i+1]
	}
	*/
	//std::cout << "\n-----PRINTING BACK PROP DEBUG------\n";
	for (int i = 0; i < numLayers; i++) {
		for (int j = 0; j < eActivation[i].getM(); j++)
			eActivation[i].set(j, 0, 0);
	}
	totalError += calcError(A);
	//std::cout << "\n-----ERROR SET, PERFORMING BACKPROP------\n";
	for (int curLayer = numLayers - 2; curLayer >= 0; curLayer--) {
		//std::cout << "\nsetting eBias for layer: " << curLayer << std::endl;
		//std::cout << "-----------------layer----------------" << std::endl;
		for (int i = 0; i < eBias[curLayer].getM(); i++) {
			//std::cout << "eBiasBuffer m: " << eBiasBuffer->getM() << ", eBiasBuffer n: " << eBiasBuffer->getN() << ", i:" << i << std::endl;

			if (curLayer == numLayers - 2 && !sigmoidOutput) {
				//eBias[curLayer].set(i, 0, eBias[curLayer].get(i, 0) + eActivation[curLayer + 1].get(i, 0));
				eBiasBuffer[curLayer].set(i, 0, eActivation[curLayer + 1].get(i, 0)); 
				//std::cout << "before add not sigmoid" << std::endl;
				
				//make temp eBias then use it to set activations
			}
			else {
				eBiasBuffer[curLayer].set(i, 0, eActivation[curLayer + 1].get(i, 0)*gatePrime(activations[curLayer + 1].get(i, 0)));
				//std::cout << "before add sigmoid" << std::endl;
			}
			
		}
		eBias[curLayer].add(&eBiasBuffer[curLayer], &eBias[curLayer]);
		/*
		std::cout << "eBias Buffer layer "<<curLayer << std::endl;
		eBiasBuffer[curLayer].print();
		std::cout << "eBias layer " << curLayer << std::endl;
		eBias[curLayer].print();
		std::cout << "\nsetting weights and prevActivations for layer: " << curLayer << std::endl;
		*/
		//weights column applied to previous activations row
		for (int i = 0; i < eWeights[curLayer].getM(); i++) {
			for (int j = 0; j < eWeights[curLayer].getN(); j++) {
				//std::cout << "\nline 75"; ading the whole weights thing
				eWeights[curLayer].set(i, j, eWeights[curLayer].get(i,j) + eBiasBuffer[curLayer].get(i,0) * activations[curLayer].get(j,0));
				//std::cout << "\nline 77";
				

				//std::cout << eBiasBuffer[curLayer].get(i, 0) * activations[curLayer].get(j, 0) << ':' << eWeights[curLayer].get(i, j)<<',';
				eActivation[curLayer].set(j, 0, eActivation[curLayer].get(j,0) + weights[curLayer].get(i, j) * eBiasBuffer[curLayer].get(i, 0));
				//std::cout << "\nline 79";
			}
			//std::cout << std::endl;
		}
		//std::cout << "activations" << std::endl;
		//activations[curLayer].print();
		//std::cout << "Done with layer: " << curLayer << std::endl;
	}
	//double stepSize = 0.0005;
	//std::cout << "\nE: " << error;
	//std::cout << "-----DONE PERFORMING BACKPROP------" << std::endl;
}
void DenseNet::updateWeights(double stepSize, int batchSize) {
	for (int l = 0; l < numLayers - 1; l++) {
		for (int i = 0; i < weights[l].getM(); i++) {
			for (int j = 0; j < weights[l].getN(); j++) {            //devide by batch size
				weights[l].set(i, j, weights[l].get(i, j) - stepSize * (eWeights[l].get(i, j)/batchSize));
			}
		}
		for (int i = 0; i < bias[l].getM(); i++) {
			bias[l].set(i, 0, bias[l].get(i, 0) - stepSize * (eBias[l].get(i, 0)/batchSize));
		}
	}
	for (int i = 0; i < numLayers - 1; i++) {
		for (int m = 0; m < eWeights[i].getM(); m++) {
			for (int n = 0; n < eWeights[i].getN(); n++) {
				eWeights[i].set(m, n, 0.0);
			}
		}
		//eWeights[i].print();
		for (int m = 0; m < eBias[i].getM(); m++) {
			eBias[i].set(m, 0, 0.0);
		}
		//eBias[i].print();
	}
	totalError = 0;
}
void DenseNet::backPropOld(Matrix* A, double stepSize) {
	//error*S()*(1-S())
	/*
	1: set last row of eActivations through calcError
	Loop for num layers{
	2: set ebias with inversegate(activations[i]) * eActivation[i]
	3: set each w[i] by corrisponding ebias[i]*activations[i] (row before the weights row)
	4: set next eActivations via transpose multiply weights[i]*activations[i+1]
	}
	*/
	//std::cout << "\n-----PRINTING BACK PROP DEBUG------\n";
	for (int i = 0; i < numLayers; i++) {
		for (int j = 0; j < eActivation[i].getM(); j++)
			eActivation[i].set(j, 0, 0);
	}
	totalError += calcError(A);
	//std::cout << "\n-----ERROR SET, PERFORMING BACKPROP------\n";
	for (int curLayer = numLayers - 2; curLayer >= 0; curLayer--) {
		//std::cout << "\nsetting eBias for layer: " << curLayer << std::endl;
		for (int i = 0; i < eBias[curLayer].getM(); i++) {
			if (curLayer == numLayers - 2 && !sigmoidOutput) {
				eBias[curLayer].set(i, 0, eActivation[curLayer + 1].get(i, 0));
			}
			else {
				eBias[curLayer].set(i, 0, eActivation[curLayer + 1].get(i, 0)*gatePrime(activations[curLayer + 1].get(i, 0)));
			}
		}
		//eBias[curLayer].print();
		//std::cout << "\nsetting weights and prevActivations for layer: " << curLayer << std::endl;
		//weights column applied to previous activations row
		for (int i = 0; i < eWeights[curLayer].getM(); i++) {
			for (int j = 0; j < eWeights[curLayer].getN(); j++) {
				//std::cout << "\nline 75";
				eWeights[curLayer].set(i, j, eBias[curLayer].get(i, 0) * activations[curLayer].get(j, 0));
				//std::cout << "\nline 77";
				eActivation[curLayer].set(j, 0, eActivation[curLayer].get(j, 0) + weights[curLayer].get(i, j) * eBias[curLayer].get(i, 0));
				//std::cout << "\nline 79";
			}
		}
	}
	//double stepSize = 0.0005;
	for (int l = 0; l < numLayers - 1; l++) {
		for (int i = 0; i < weights[l].getM(); i++) {
			for (int j = 0; j < weights[l].getN(); j++) {
				weights[l].set(i, j, weights[l].get(i, j) - stepSize * (eWeights[l].get(i, j)));
			}
		}
		for (int i = 0; i < bias[l].getM(); i++) {
			bias[l].set(i, 0, bias[l].get(i, 0) - stepSize * (eBias[l].get(i, 0)));
		}
	}
	//std::cout << "\nE: " << error;
}
double DenseNet::calcError(Matrix* A) {
	double error = 0;
	for (int i = 0; i < layerList[numLayers-1]; i++) {
		//std::cout << "\n---------FINDING ERROR---------\n";
		eActivation[numLayers-1].set(i, 0, activations[numLayers - 1].get(i,0) - A->get(i,0));
		error += 0.5*pow(eActivation[numLayers - 1].get(i,0),2);
	}
	return error;
}
/*
void DenseNet::gate(Matrix* A) {
//derivative of sigmoid S() is S()*(1-S())
for (int i = 0; i < A->getM(); i++) {
A->set(i, 0, 1.0/(1.0+exp(-1*(A->get(i, 0)))));
}
}
double DenseNet::gatePrime(double a) {
return a * (1 - a);
}*/
void DenseNet::print() {
	std::cout<<"will this print"<<std::endl;
	std::cout << "\n\n ----------PRINTING A DENSE NET----------"<<std::endl;
	std::cout<<"hi"<<std::endl;
	std::cout<<"numLayers: "<<numLayers<<std::endl;
	for (int i = 0; i < numLayers; i++) {
		std::cout << "activations: " << i << std::endl;
		activations[i].print();
		if (i < numLayers - 1) {
			std::cout << "weights: " << i << std::endl;
			weights[i].print();
			std::cout << "bias: " << i << std::endl;
			bias[i].print();
		}

	}
	std::cout << "\n\n -------------END PRINT DENSE NET-------------\n\n";
}
void DenseNet::printGradient() {
	std::cout << "\n\n ----------PRINTING GRADIENT----------\n\n";
	for (int i = 0; i < numLayers; i++) {
		std::cout << "eActivation: " << i << std::endl;
		eActivation[i].print();
		if (i < numLayers - 1) {
			std::cout << "eWeights: " << i << std::endl;
			eWeights[i].print();
			std::cout << "eBias: " << i << std::endl;
			eBias[i].print();
		}
	}
	std::cout << "\n\n -------------END PRINT GRADIENT-------------\n\n";
}
void DenseNet::save(){
	std::cout<<"saving net"<<std::endl;
	std::ofstream outfile;
	outfile.open(name);
	
	if(outfile.is_open()){
		outfile<<numLayers<<','<<sigmoidOutput<<',';
		for(int i = 0; i < numLayers; i++){
			if(i>0)outfile<<',';
			outfile<<layerList[i];
		}
		outfile<<std::endl;
		for(int i = 0; i < numLayers-1; i++){
			for(int j = 0; j < weights[i].getM(); j++){
				for(int k = 0; k < weights[i].getN();k++){
					if(k>0) outfile<<',';
					outfile<<weights[i].get(j,k);
				}
				outfile<<std::endl;
			}
		}
		for(int i = 0; i < numLayers-1; i++){
			for(int j = 0; j < bias[i].getM(); j++){
				if(j>0) outfile<<',';
				outfile<<bias[i].get(j,0);
			}
			if(i < numLayers-1) outfile<<std::endl;
		}
		outfile.close();
	} else{
		std::cout<<"ERROR could not open/create file"<<std::endl;
	}
}
void DenseNet::setName(char* nm){
	name = nm;
}