#include "Reader.hh"
#include "tools/help.hh"
#include "tools/Logger.hh"
#include <fstream>


io::Reader::Reader(std::string i_backupFilename, std::string i_outputFilename, int i_rank, int i_mpiSizeCurrent, int i_blockPosX, int i_blockPosY):
                    backupFilename(i_backupFilename), outputFilename(i_outputFilename),
                    rank(i_rank), mpiCurrentSize(i_mpiSizeCurrent),
                    blockPosX(i_blockPosX), blockPosY(i_blockPosY){
    
    readMetadataFile(i_backupFilename + "_metadata");

    std::string backupFileName = generateBaseFileName(backupFilename, blockPosX, blockPosY);
    std::string outputFileName = generateBaseFileName(outputFileName, blockPosX, blockPosY);
    std::ifstream src(backupFilename, std::ios::binary);
	std::ofstream out(outputFileName, std::ios::binary);
	out << src.rdbuf();
    
    scenario = new SWE_LoadNetCdfScenario(backupFileName, totalTime, boundaryTypes, boudaryPositions);

    
}

io::Reader::~Reader(){
    
}

//TODO implement real parser for those files
void io::Reader::readMetadataFile(std::string filename){
    mpiExpectedSize = std::stoi(readConfigureFileValue(filename, "ranks"));
    sizeX = std::stoi(readConfigureFileValue(filename, "grid_size_x"));
    sizeY = std::stoi(readConfigureFileValue(filename, "grid_size_y"));
    totalTime = std::stof(readConfigureFileValue(filename, "total_time"));
    currentTime = std::stof(readConfigureFileValue(filename, "current_time"));
    remainingCheckpoints = std::stoi(readConfigureFileValue(filename, "checkpoints_unfinished"));
    for(int i = 0; i <= 3; i++){
        std::string boundaryType = "boundary_type_" + std::to_string(i);
        std::string boudaryPosition = "boundary_position_" + std::to_string(i);

        BoundaryType type = static_cast<BoundaryType> (std::stoi(readConfigureFileValue(filename, boundaryType)));
        float pos = std::stof(readConfigureFileValue(filename, boudaryPosition));

        boundaryTypes.push_back(type);
        boudaryPositions.push_back(pos);
    }
    
}

 float io::Reader::getRemainingTime(){
        return totalTime - currentTime;
}
        
float io::Reader::getCurrentTime(){
    return currentTime;
}
int io::Reader::getRemainingCheckpoints(){
    return remainingCheckpoints;
}

SWE_Scenario *io::Reader::getScenario(){
    return scenario;
}

int io::Reader::getGridSizeX(){
    return sizeX;
}

int io::Reader::getGridSizeY(){
    return sizeY;
}