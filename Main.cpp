/*
* Programming Language: Visual C++ 14.1
* Author: Kaitlyn Deppmeier
* Class: CPSC 481
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

//#include <stdio.h>
//#include <stdlib.h> 

#include <time.h>



//Class meant to represent an entry from the seed data file
class SeedEntry
{
public:
	float attributes[7];  //The attributes of the seed

	int seedType;  //The seed's type
	int centroidLabel; //The cluster the seed is placed into
	bool isKnown; //Is the seed type "known"?  (Unknown seeds still have a type, though that data won't be used

	SeedEntry()
	{

	}

	SeedEntry(float at1, float at2, float at3, float at4, float at5, float at6, float at7, int st, bool known)
	{
		attributes[0] = at1;
		attributes[1] = at2;
		attributes[2] = at3;
		attributes[3] = at4;
		attributes[4] = at5;
		attributes[5] = at6;
		attributes[6] = at7;
		seedType = st;
		centroidLabel = -1;
		isKnown = known;
	}
};

//Vector containing all of the seed data entries
std::vector<SeedEntry> seedData;

//Reads the file and stores its data as numbers
bool readFile(std::string filename, bool known)
{
	//Reads in the data from the seed file
	std::string line;
	std::ifstream f(filename);
	if (f.is_open())
	{
		//Do this with substrings by finding numbers instead?
		while (getline(f, line))
		{
			float arr[7]; //Will contain the seven attributes of the seed
			int seed = -1; //The seed type attribute.  -1 if not yet set


			int arrIndex = 0; //Index of arr
			int begin = -1; //Start of a number/substring of line
			int end; //End of a number/substring of line
			for (int i = 0; i < line.length(); i++)
			{
				//If the start of a number is reached
				if (begin == -1 && (isdigit(line[i]) || line[i] == '.'))
				{
					begin = i;
				}
				//If the end of a number is reached
				if (!isdigit(line[i]) && line[i] != '.')
				{
					if (begin != -1) //If there is a number - in case of two spaces in a row
					{
						end = i; //End is set to current index
						std::string attribute = line.substr(begin, end); //Extracts the number as a substring of line
						if (arrIndex < 7)  //If the attributes have not all been set.
						{
							arr[arrIndex] = std::stof(attribute);  //Casts the attribute as a float
							arrIndex++;
							begin = -1; //Resets the begin index
						}
						else if (seed == -1) //If the attributes are all read but the seed hasn't
						{
							arr[arrIndex] = std::stof(attribute);
							seed = std::stoi(attribute);
							begin = -1;
						}
						else
						{
							std::cout << "More fields than expected.  Double check file." << std::endl;
						}
					}
				}
			}
			if (begin != -1 && seed == -1)
			{
				end = line.length();
				std::string attribute = line.substr(begin, end);
				seed = std::stoi(attribute);
			}

			SeedEntry newSeed(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], seed, known);
			if (known)
			{
				newSeed.centroidLabel = seed;
			}
			seedData.push_back(newSeed);
		}
		f.close();
	}
	else
	{
		return false;
	}
}

//Uses K-means clustering on the data set (First two attributes for now)
void kMeans()
{

	float coords[7][3]; //Coordinates for the three centroids.  There are seven per centroid, one for each attribute
	for (int i = 0; i < 7; i++) //Sets coordinates to 0 to start
	{
		for (int j = 0; j < 3; j++)
		{
			coords[i][j] = 0;
		}
	}

	int numForMean[] = { 0, 0, 0 };  //Number of each known seed type.  Used to calculate average

	//Uses mean of known seeds to generate the initial centroids
	for (int i = 0; i < seedData.size(); i++)
	{
		SeedEntry currSeed = seedData.at(i);
		if (currSeed.isKnown)
		{
			for (int j = 0; j < 7; j++)
			{
				coords[j][currSeed.centroidLabel - 1] += currSeed.attributes[j];
			}
			numForMean[currSeed.centroidLabel - 1] += 1;
		}
	}	
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (numForMean[j] > 0)
			{
				//The initial centroids are the means of the known seeds
				coords[i][j] /= numForMean[j];
			}
			else
			{
				std::cout << "ERROR, knownSeeds.txt doesn't have samples for all three wheat types." << numForMean[j] << std::endl;
				return;
			}
		}
	}

	//Calculates normalization values based on the max and mins of each attribute
	float minAllAt[] = { 99999, 99999, 99999, 99999, 99999, 99999, 99999 };
	float maxAllAt[] = { 0, 0, 0, 0, 0, 0, 0 };
	float normVal[7];  //Normalization values.  Unused in favor of standardization.

	for (int i = 0; i < seedData.size(); i++)
	{
		SeedEntry currSeed = seedData.at(i);

		for (int j = 0; j < 7; j++)
		{
			if (currSeed.attributes[j] > maxAllAt[j])
			{
				maxAllAt[j] = currSeed.attributes[j];
			}
			if (currSeed.attributes[j] < minAllAt[j])
			{
				minAllAt[j] = currSeed.attributes[j];
			}
		}
	}
	std::cout << "Normalization divisor: " << std::endl;
	for (int i = 0; i < 7; i++)
	{
		normVal[i] = 1 / (maxAllAt[i] - minAllAt[i]);
		std::cout << normVal[i] << " ";
	}
	std::cout << "\n" << std::endl;

	//Finds the standard deviations - used to standardize the data
	float standardDev[] = { 0, 0, 0, 0, 0, 0, 0 };
	float mean[] = { 0, 0, 0, 0, 0, 0, 0 };
	int count = 0;
	for (int i = 0; i < seedData.size(); i++)  //Gets the mean
	{
		SeedEntry currSeed = seedData.at(i);
		for (int j = 0; j < 7; j++)
		{
			mean[j] += currSeed.attributes[j];
		}
		count++;
	}
	for (int i = 0; i < 7; i++)
	{
		mean[i] /= count;
	}
	for (int i = 0; i < seedData.size(); i++)  //Gets the standard deviation
	{
		SeedEntry currSeed = seedData.at(i);
		for (int j = 0; j < 7; j++)
		{
			standardDev[j] += std::powf(mean[j] - currSeed.attributes[j], 2);
		}
	}
	std::cout << "Standard deviation divisors" << std::endl;
	for (int i = 0; i < 7; i++)
	{
		standardDev[i] /= count;
		standardDev[i] = 1 / std::sqrtf(standardDev[i]);
		std::cout << standardDev[i] << " ";
	}
	std::cout << "\n" << std::endl;

	//Prints initial centroids
	std::cout << "Initial Centroids:" << std::endl;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			std::cout << coords[j][i] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	int iterations = 0;

	//Increasing the size of knownSeeds.txt seems to lower the number of mismatches
	//K means (using set number of iterations)
	while (iterations < 100)
	{
		//label seed data points based on closest centroid
		for (int i = 0; i < seedData.size(); i++)
		{
			SeedEntry currSeed = seedData.at(i);
			if (currSeed.isKnown == false)
			{
				//Finds the centroid that is closest to the seed data point
				float distFrom1 = 0;
				float distFrom2 = 0;
				float distFrom3 = 0;
				for (int j = 0; j < 7; j++)
				{
					//Uses standard deviations to adjust the distance in order to make the results more accurate
					distFrom1 += std::pow((standardDev[j] * (currSeed.attributes[j] - coords[j][0])), 2);
					distFrom2 += std::pow((standardDev[j] * (currSeed.attributes[j] - coords[j][1])), 2);
					distFrom3 += std::pow((standardDev[j] * (currSeed.attributes[j] - coords[j][2])), 2);
				}
				distFrom1 = std::sqrtf(distFrom1);
				distFrom2 = std::sqrtf(distFrom2);
				distFrom3 = std::sqrtf(distFrom3);

				//Assigns centroid label to currSeed - the label will become the closest centroid to currSeed
				float smallest = distFrom1;
				seedData.at(i).centroidLabel = 1;
				if (distFrom2 < smallest)
				{
					smallest = distFrom2;
					seedData.at(i).centroidLabel = 2;
				}
				if (distFrom3 < smallest)
				{
					smallest = distFrom3;
					seedData.at(i).centroidLabel = 3;
				}
			}
		}

		//assign centroids based on datapoint labels by finding the averages of the clusters
		//sums the total seeds per cluster and their attributes
		int numClus[] = { 0, 0, 0 };
		float totalAt1[] = { 0, 0, 0 };
		float totalAt2[] = { 0, 0, 0 };
		float totalAt3[] = { 0, 0, 0 };
		float totalAt4[] = { 0, 0, 0 };
		float totalAt5[] = { 0, 0, 0 };
		float totalAt6[] = { 0, 0, 0 };
		float totalAt7[] = { 0, 0, 0 };
		for (int i = 0; i < seedData.size(); i++)
		{
			SeedEntry currSeed = seedData.at(i);
			if (currSeed.centroidLabel >= 1 && currSeed.centroidLabel <= 3)
			{
				numClus[currSeed.centroidLabel - 1] += 1;
				totalAt1[currSeed.centroidLabel - 1] += currSeed.attributes[0];
				totalAt2[currSeed.centroidLabel - 1] += currSeed.attributes[1];
				totalAt3[currSeed.centroidLabel - 1] += currSeed.attributes[2];
				totalAt4[currSeed.centroidLabel - 1] += currSeed.attributes[3];
				totalAt5[currSeed.centroidLabel - 1] += currSeed.attributes[4];
				totalAt6[currSeed.centroidLabel - 1] += currSeed.attributes[5];
				totalAt7[currSeed.centroidLabel - 1] += currSeed.attributes[6];
			}
			else
			{
				//In case a centroid somehow wasn't labeled
				std::cout << "???" << std::endl;
			}
		}
		//Finds the means and creates the new centroids
		for (int i = 0; i < 3; i++)
		{
			if (numClus[i] > 0)
			{
				coords[0][i] = (float)totalAt1[i] / numClus[i];
				coords[1][i] = (float)totalAt2[i] / numClus[i];
				coords[2][i] = (float)totalAt3[i] / numClus[i];
				coords[3][i] = (float)totalAt4[i] / numClus[i];
				coords[4][i] = (float)totalAt5[i] / numClus[i];
				coords[5][i] = (float)totalAt6[i] / numClus[i];
				coords[6][i] = (float)totalAt7[i] / numClus[i];
			}
		}

		iterations++;  //Incriments iterations
	}

	return;
}

//Writes the results to a file
void writeFile(std::string filename)
{
	int numMisMatch = 0;
	int numTotal = 0;

	std::ofstream file (filename);
	if (file.is_open())
	{
		for (int i = 0; i < seedData.size(); i++)
		{
			//Prints the attributes of each seed line by line
			file << seedData[i].attributes[0] << "\t" << seedData[i].attributes[1] << "\t" << seedData[i].attributes[2] << "\t" << seedData[i].attributes[3] << "\t";
			file << seedData[i].attributes[4] << "\t" << seedData[i].attributes[5] << "\t" << seedData[i].attributes[6] << "\t" << seedData[i].seedType;
			file << "\t" << seedData[i].centroidLabel << "\t" << seedData[i].isKnown << std::endl;

			//Calculates mismatches for the unknown seeds.  Used for refining the algorithm.
			if (!seedData[i].isKnown)
			{
				if (seedData[i].centroidLabel != seedData[i].seedType)
				{
					numMisMatch++;
				}
				numTotal++;
			}
		}
	}

	//Prints out mismatches
	std::cout << "Number mismatched: " << numMisMatch << "/" << numTotal << std::endl;
}

int main()
{
	//Reads the unknown and known seeds file.  Bool refers to whether the algorithm should
	//treat the seed's categorization as known or unknown
	bool fileRead = readFile("seeds.txt", false);
	bool file2Read = readFile("knownSeeds.txt", true);

	if (fileRead & file2Read)
	{
		std::cout << "File read." << std::endl;

		//Calls K means
		kMeans();

		//Outputs results
		std::cout << "Attributes" << std::endl;
		for (int i = 0; i < seedData.size(); i++)
		{
			std::cout << seedData[i].attributes[0] << "\t" << seedData[i].attributes[1] << "\t" << seedData[i].attributes[2] << "\t" << seedData[i].attributes[3] << "\t";
			std::cout << seedData[i].attributes[4] << "\t" << seedData[i].attributes[5] << "\t" << seedData[i].attributes[6] << "\t" << seedData[i].seedType;
			std::cout << "\t" << seedData[i].centroidLabel << std::endl;
		}

		std::ofstream file{ "results.txt" };
		writeFile("results.txt");
		std::cout << "Clustering successful.  Press Enter to continue." << std::endl;
	}
	else
	{
		std::cout << "Unable to open file.  Press Enter to continue." << std::endl;
	}

	std::cin.get();
	return 0;
}

