#ifndef NETWORK_H
#define NETWORK_H

/*
Class that creates the fixed household network where exchange of varieties
happens. It can create three types of networks, based on Erdos-Renyi,
Watts-Strogatz and Barabasi-Albert models.
*/

class Network{
private:
  int m_numberHousehold;
  int m_mSF;
  int m_kWT;
  float m_betaWT;
  float m_probabilyConnectionER; // Probability of connection
  void createRandomNetwork(void);
  void createWTNetwork(void);
  void createSFNetwork(void);
  std::vector<float> computeCumulativeDistribution();
public:
  Network(int t_networkType, int t_mSF, int t_kWT, float t_betaWT,
      float t_probabilyConnectionER, int t_numberHousehold);
  std::vector<std::vector<int> > indexLinkedHDs;
  void printNetwork(void);
};

// Network constructor. initialize parameters, create network according to
// t_networkType
  Network::Network(int t_networkType, int t_mSF, int t_kWT, float t_betaWT,
      float t_probabilyConnectionER, int t_numberHousehold)
  : m_numberHousehold(t_numberHousehold)
  , m_mSF(t_mSF)
  , m_kWT(t_kWT)
  , m_betaWT(t_betaWT)
  , m_probabilyConnectionER(t_probabilyConnectionER)
  , indexLinkedHDs(t_numberHousehold)
{
  if(t_networkType == 0)
    createRandomNetwork();
  else if(t_networkType == 1)
    createWTNetwork();
  else if(t_networkType == 2)
    createSFNetwork();
  else{
    cout << "ERROR: invalid network type." << endl;
    exit(-1);
  }
}

// Create indexLinkedHDs based on a random network (Erdos-Renyi Model)
void Network::createRandomNetwork(){
  for(int i = 0; i < m_numberHousehold; ++i){
    for(int j = i+1; j < m_numberHousehold; ++j)
      if(uniFLOAT(rand64) < m_probabilyConnectionER){
        indexLinkedHDs[i].push_back(j);
        indexLinkedHDs[j].push_back(i);
      }
  }
  for(int i = 0; i < m_numberHousehold; ++i){
    if(indexLinkedHDs[i].empty()){
      int j = floor(uniFLOAT(rand64) * m_numberHousehold);
      while(i == j)
        j = floor(uniFLOAT(rand64) * m_numberHousehold);
      indexLinkedHDs[i].push_back(j);
      indexLinkedHDs[j].push_back(i);
    }
  }
}

// Create indexLinkedHDs based on a small-world network (Watts–Strogatz Model)
void Network::createWTNetwork(){

  // Create matrix and make ring network
  std::vector<int> matrix(m_numberHousehold*m_numberHousehold, 0);
  for(int lin = 0; lin < m_numberHousehold; ++lin){
    for(int addTerm = 1; addTerm < 1 + m_kWT / 2; ++addTerm){
      int colL = lin - addTerm;
      int colR = lin + addTerm;
      if(colL < 0)
        colL = colL + m_numberHousehold;
      if(colR > m_numberHousehold - 1)
        colR = colR - m_numberHousehold;
      if(colL > lin)
        matrix[lin*m_numberHousehold+colL] = 1;
      if(colR > lin)
        matrix[lin*m_numberHousehold+colR] = 1;
    }
  }

  // Remove links with probability betaWT
  int removedLinks = 0;
  std::uniform_int_distribution<long> uniIntHD(0,m_numberHousehold-1);
  for(int i = 0; i < m_numberHousehold*m_numberHousehold; ++i){
    if(matrix[i] == 1 && uniFLOAT(rand64) < m_betaWT){
      matrix[i] = 0;
      ++removedLinks;
      }
  }

  // Rewire randomly
  for(int i = 0; i < removedLinks; ++i){
    int newHD1 = uniIntHD(rand64);
    int newHD2 = uniIntHD(rand64);
    while(newHD2 == newHD1 || matrix[newHD1*m_numberHousehold+newHD2] == 1
        || matrix[newHD2*m_numberHousehold+newHD1] == 1)
      newHD2 = uniIntHD(rand64);
    matrix[newHD1*m_numberHousehold+newHD2] = 1;
  }

  // Sum transpose
  for(int lin = 0; lin < m_numberHousehold; ++lin){
    for(int col = lin + 1; col < m_numberHousehold; ++col){
      matrix[lin*m_numberHousehold+col] =
        matrix[lin*m_numberHousehold+col] +
        matrix[col*m_numberHousehold+lin];
      matrix[col*m_numberHousehold+lin] =
        matrix[lin*m_numberHousehold+col];
    }
  }

  // Fill indexLinkedHDs vector of vectors
  for(int i = 0; i < m_numberHousehold; ++i){
    for(int j = 0; j < m_numberHousehold; ++j)
      if(matrix[i*m_numberHousehold+j] == 1)
        indexLinkedHDs[i].push_back(j);
  }

  // If there is an unnconnect HD, one additional connection is made
  for(int i = 0; i < m_numberHousehold; ++i){
    if(indexLinkedHDs[i].empty()){
      int newHD = uniIntHD(rand64);
      indexLinkedHDs[i].push_back(newHD);
      indexLinkedHDs[newHD].push_back(i);
    }
  }
}

// Create indexLinkedHDs based on a scale-free network (Barabasi–Albert Model)
void Network::createSFNetwork(){
  int m0 = 2;
  int t = m_numberHousehold - m0;
  std::vector<float> cumulative;
  indexLinkedHDs = std::vector<std::vector<int> >(m0);
  indexLinkedHDs[0].push_back(1);
  indexLinkedHDs[1].push_back(0);
  int lastNode = m0;
  for(int i = 0; i < t; ++i){
    cumulative = computeCumulativeDistribution();
    indexLinkedHDs.push_back(std::vector<int>(0));
    std::vector<int> newNodes;
    for(int j = 0; j < m_mSF; ++j){
      int node = 0;
      float randNumber = uniFLOAT(rand64);
      while(cumulative[node] < randNumber)
        ++node;
      if(std::find(newNodes.begin(), newNodes.end(), node) != newNodes.end())
        --j;
      else{
        indexLinkedHDs[lastNode].push_back(node);
        indexLinkedHDs[node].push_back(lastNode);
        newNodes.push_back(node);
      }
    }
    ++lastNode;
  }
}

// Compute the cumulative vector based on the degree of the node
std::vector<float> Network::computeCumulativeDistribution(void){
  std::vector<float> cumulative(indexLinkedHDs.size());
  cumulative[0] = indexLinkedHDs[0].size();
  for(uint i = 1; i < indexLinkedHDs.size(); ++i)
    cumulative[i] = cumulative[i-1] + indexLinkedHDs[i].size();
  for(uint i = 0; i < indexLinkedHDs.size(); ++i)
    cumulative[i] /= cumulative.back();
  cumulative.back() = 1.0;
  return cumulative;
}

void Network::printNetwork(void){
  for(uint i = 0; i < indexLinkedHDs.size(); ++i){
    cout << "HD number " << i << ": ";
    for(auto j : indexLinkedHDs[i])
      cout << j << ", ";
    cout << endl;
  }
}

#endif
