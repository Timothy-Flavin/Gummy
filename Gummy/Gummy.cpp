#include"Gummy.h"
#include"Timer.h"

Gummy::Gummy(){
	setNetFileName(new char['t','e','s','t','N','e','t','.','t','x','t']);

}

//make it take a split int n to split te file into n folds
//read the first file 
csv* Gummy::readCSV(char* fileName) {
	
	int curLine = 0;
	//C code
	curLine = 0;
	FILE *inffile = fopen(fileName, "r");
	int ch;

	while (EOF != (ch = getc(inffile)))
		if ('\n' == ch)
			++curLine;
	if (EOF == fclose(inffile)) {
		std::cout << "fclose error" << std::endl;
	}
	//end C code
	csv* file = new csv;
	file->name = fileName;
	file->numLines = curLine;
	//std::cout<<"made num lines"<<file->numLines<<std::endl;
	file->data = new std::string[curLine+1];
	curLine = 0;
	std::ifstream infile;
	infile.open(fileName);
	//std::cout<<"made empty file"<<std::endl;
	while (std::getline(infile, file->data[curLine])) {
		//std::cout << file->data[curLine] << std::endl;
		curLine++;
	}
	//std::cout<<"filled file"<<std::endl;
	int numVals = 1;
	for (int i = 0; i < file->data[0].size(); i++) {
		if (file->data[0].at(i) == ',') {
			numVals++;
		}
	}
	//std::cout<<"got num vals"<<std::endl;
	//std::cout << "\nnumVals: " << numVals<<std::endl;
	file->numVals = numVals;
	infile.close();
	return file;
}

//make another constructor for if there is a number of folds
void Gummy::updateTrainingData(bool numbersOnly) {
	trainingData = readCSV(csvFileName);
	if(numbersOnly)
		csvToDouble(trainingData);
}
void Gummy::csvToDouble(csv* file) {
	//std::cout << "------------------------CSV TO DOUBLE---------------\n";
	file->numData = new double*[file->numLines];
	for (int i = 0; i < file->numLines; i++) {
		//std::cout << "starting thing\n";
		int valCount = 1;
		for (int j = 0; j < file->data[i].size(); j++) {
			if(file->data[i].at(j) == ','){
				valCount++;
			}
		}
		file->numData[i] = new double[valCount];
		//std::cout << "after error thing\n";
		int commaPos = 0;
		int vals = 0;
		for (int j = 0; j < file->data[i].size(); j++) {
			if (file->data[i].at(j) == ',') {
				//std::cout<<"commaPos: "<<commaPos<<", j: "<<j<<", j-commaPos: "<<j-commaPos<<std::endl;
				//std::cout << file->data[i].substr(commaPos, j - commaPos)<<std::endl;
				try{
				file->numData[i][vals] = std::stod(file->data[i].substr(commaPos, j - commaPos));
				//std::cout<<file->numData[i][vals]<<',';
				} catch(std::invalid_argument){
					std::cout<<"i: "<<i<<", comma pos: "<<commaPos<<", j: "<<j<<std::endl;
					std::cout<<"file data: "<<file->data[i].substr(commaPos, j-commaPos);
				}
				commaPos = j+1;
				vals++;
			}
			else if (j == file->data[i].size() - 1) {
				//std::cout << "\nsecond if";
				//std::cout<<"second if " << file->data[i].substr(commaPos)<<std::endl;
				file->numData[i][vals] = std::stod(file->data[i].substr(commaPos));
				//std::cout<<file->numData[i][vals]<<',';
				commaPos = j;
				vals++;
			}
		}
		//std::cout<<std::endl;
	}
}

DenseNet* Gummy::manualInitDense(char* fileName, char* nFileName, int ntype, int numLayers, int* layerSizes, bool sigmoid){
	csvFileName = fileName;
	type = ntype;
	netFileName=nFileName;
	return new DenseNet(numLayers, layerSizes, sigmoid, netFileName);
}
RecurrentNet* Gummy::manualInitRecurrent(char* fileName, char* nFileName, int numLayers, int* layerSizes, bool sigmoid, recurrentRelation** relations, int numRelations, int truncNum) {
	csvFileName = fileName;
	netFileName = nFileName;
	return new RecurrentNet(numLayers, layerSizes, sigmoid, nFileName, relations, numRelations, truncNum);
	//RecurrentNet(int nl, int*ll, bool so, char* nm, recurrentRelation** relations, int numRelations, int truncNum);
}
NeuralNetwork* Gummy::userInit() {
	NeuralNetwork* net;
	std::cout<< "\nEnter the file name to save your net: ";
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	netFileName = new char[20];
	std::cin.getline(netFileName, 20);
	std::cout << "\nWhat type of Neural net do you want?\n1: Dense, 2: Recurrent(inProgress), 3: convolutional(NA)\n";
	std::cin >> type;
	int numLayers = 0;
	int*layerSizes=NULL;
	bool sigmoidOutput = true;
	std::cout << "\nHow many layers do you want?\n";
	std::cin >> numLayers;
	layerSizes = new int[numLayers];
	std::cout << "number of nodes in layer 0 and last layer must \nadd up to number of values in each row of csv file\n";
	for (int i = 0; i < numLayers; i++) {
		std::cout << "how many nodes in layer " << i << std::endl;
		std::cin >> layerSizes[i];
	}
	std::cout << "\nChoose output mode: 1: sigmoid(more gate options coming), 0: raw\n";
	std::cin >> sigmoidOutput;

	if (type == 2) {
		//RecurrentNet::RecurrentNet(int nl, int*ll, bool so, char* nm, recurrentRelation** relations, int numRelations, int truncNum) :DenseNet(nl, ll, so, nm) { //multiple relations
		int numRelations, truncNum;
		std::cout << "how many recurrent relations do you want in your network?";
		std::cin >> numRelations;
		recurrentRelation** relations = new recurrentRelation*[numRelations];
		for (int i = 0; i < numRelations; i++) {
			relations[i] = new recurrentRelation;
			std::cout << "What do you want to be the input layer number of relation " << i;
			std::cin >> relations[i]->inputLayer;
			std::cout << "What do you want to be the output layer number of relation " << i;
			std::cin >> relations[i]->outputLayer;
			std::cout << "What recurrent depth do you want in timesteps for relation " << i;
			std::cin >> relations[i]->timeDistance;
		}
		std::cout << "How many timesteps do you want truncated BBTT to remember?";
		std::cin >> truncNum;
		net = new RecurrentNet(numLayers, layerSizes, sigmoidOutput, netFileName, relations, numRelations, truncNum);
		//std::cout << "Testing print relations in gummy" << std::endl;
		//net->printRelations();
	}
	else {
		net = new DenseNet(numLayers, layerSizes, sigmoidOutput, netFileName);
	}
	std::cout << "FINISHED GATHERING DATA\n" << csvFileName << std::endl;

	//net->print();
	char traininginit = 'f';
	std::cout << "Would you like to initialize csv data here? ";
	std::cin >> traininginit;
	if (traininginit == 'y' || traininginit == 'Y' || traininginit == 't' || traininginit == 'T') {
		std::cout << "reading file" << std::endl;
		trainingData = readCSV(csvFileName);
		csvToDouble(trainingData);
	}
	std::cout<<"FINISHED MAKING DENSE NET\n";
	return net;
}
//Stochastic gradient descent
void Gummy::train(NeuralNetwork* net, int batchSize) {
	//int*rowNums = new int[batchSize];
	std::cout<<"training net"<<std::endl;
	int percentNum = 1;
	if (numIterations >= 100) {
		percentNum = numIterations / 100;
	}
	std::cout<<"assigned percent "<<numIterations<<std::endl;
	//net->print();
	double* tempRow = nullptr;
	int tailNum = trainingData->numLines;
	int rowNum = 0;
	int numIn = net->getNumInputs();
	int numOut = net->getNumOutputs();
	//std::cout<<"got in and outputs"<<std::endl;
	Matrix* inputMatrix = new Matrix(numIn,1);
	Matrix* outputMatrix = new Matrix(numOut,1);
	Matrix* netOutput = new Matrix(numOut, 1);
	//std::cout<<"made training matrices"<<std::endl;
	for (int i = 0; i < numIterations; i++) {
		for (int j = 0; j < batchSize; j++) {
			
			//rowNum = rand() % tailNum; //trainingData->numLines - 
			
			rowNum = rand() % tailNum;
			//std::cout<<"picked a row: "<<rowNum <<": "<< trainingData->numData[rowNum][0] << ", " << trainingData->numData[rowNum][1] << ", " << trainingData->numData[rowNum][2] << std::endl;
			for(int k = 0; k < trainingData->numLines;k++)
				//std::cout << trainingData->numData[k][0]<<", "<<trainingData->numData[k][1]<<", "<< trainingData->numData[k][2]<<std::endl;
			//std::cout << std::endl;
			for (int k = 0; k < numIn; k++) {
				inputMatrix->set(k, 0, trainingData->numData[rowNum][k]);
				//std::cout << trainingData->numData[rowNums[j]][k] << ',';
			}
			//std::cout << "output:";
			for (int k = 0; k < numOut; k++) {
				outputMatrix->set(k, 0, trainingData->numData[rowNum][k + numIn]);
				//std::cout << trainingData->numData[rowNums[j]][k+numIn] << ',';
			}
			//std::cout << std::endl;
			//std::cout<<"set input and outputs"<<std::endl;

			net->feedForward(inputMatrix);
			//std::cout<<"fed forward"<<std::endl;
			net->backProp(outputMatrix, stepSize);
			//if (i == 0 && j == 0) {
				//std::cout << "---------------------new back prop----------------" << std::endl;
				//net->printGradient();
				//net->feedForward(inputMatrix);
				//net->backPropOld(outputMatrix, stepSize);
				//std::cout << "---------------------old back prop----------------" << std::endl;
				//net->printGradient();
			//}
			//std::cin.get();
			//std::cin.get();
			//std::cout<<"-----------------------back propped------------------"<<std::endl;
			tailNum--;
			tempRow = trainingData->numData[rowNum];
			trainingData->numData[rowNum] = trainingData->numData[tailNum];
			trainingData->numData[tailNum] = tempRow;
			if (tailNum == 0) {
				tailNum = trainingData->numLines;
			}
					   //we picked a randome legal row num, at the end of the
					   //training loop we must swap the data at row num and
					   //the data at tail num

		}
		//std::cout << "------------------------END BATCH-------------------"<<std::endl;
		if (i%percentNum == 0) {
			//int errorBatchSize = 100;
			std::cout << "avg error for this batch: " << net->getError() / batchSize << std::endl;
			std::cout << "percent done: " << i / percentNum << "%\n";


			/*int randomEntry = rand()%trainingData->numLines;
			double error = 0;
			for(int rowNum = 0; rowNum < errorBatchSize; rowNum++){
			randomEntry = rand()%trainingData->numLines;
			for(int j = 0; j < numIn; j++){
			inputMatrix->set(j,0,trainingData->numData[randomEntry][j]);
			}
			for(int j = 0; j < numOut; j++){
			outputMatrix->set(j,0,trainingData->numData[randomEntry][j+numIn]);
			}
			net->feedForward(inputMatrix);
			error+=net->calcError(outputMatrix);
			}
			std::cout<<"error: "<<error/ errorBatchSize <<" batch size = "<<errorBatchSize<<std::endl;
			*/
		}
		net->updateWeights(stepSize, batchSize);

		
	}
	for(int i = 0; i < 5; i++){
		rowNum = rand() % trainingData->numLines;
		std::cout<<"input: ";
		for(int j = 0; j < numIn; j++){
			inputMatrix->set(j,0,trainingData->numData[rowNum][j]);
			std::cout<<inputMatrix->get(j,0)<<", ";
		}
		std::cout<<std::endl;
		std::cout<<"desired output: ";
		for(int j = 0; j < numOut; j++){
			outputMatrix->set(j,0,trainingData->numData[rowNum][j+numIn]);
			std::cout<<outputMatrix->get(j,0)<<", ";
		}
		std::cout<<std::endl;
		std::cout<<"actual output: ";
		netOutput = net->feedForward(inputMatrix);
		for(int j = 0; j < netOutput->getM(); j++){
			std::cout<<netOutput->get(j,0)<<", ";
		}
		std::cout<<std::endl;
	}
	//delete[] rowNums;
	//rowNums = 0;
}

void Gummy::train(NeuralNetwork* net) {
	train(net, trainingData->numLines);
}

void Gummy::saveNet(NeuralNetwork* net){
	net->save();
}

DenseNet* Gummy::loadNet(char* fileName){
	csv* loadnet = readCSV(fileName);
	csvToDouble(loadnet);
	
	DenseNet* net = new DenseNet(loadnet);
	return net;
}


