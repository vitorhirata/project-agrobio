#ifndef NETWORK_H
#define NETWORK_H

class Network{
private:
  int m_numberDomesticUnity;
  int m_mSF;
  int m_kWT;
  float m_betaWT;
  float m_probabilyConnectionER; // Probability of connection
  void createRandomNetwork(void);
  void createWTNetwork(void);
  void createSFNetwork(void);
  std::vector<float> computeCumulativeDistribution();
public:
  Network(int t_networkType, int t_mSF, int t_kWT, int t_betaWT, int t_probabilyConnectionER, int t_numberDomesticUnity);
  std::vector<std::vector<int> > indexLinkedDUs;
  void printNetwork(void);
};

// Network constructor. initialize parameters, create network according to t_networkType
  Network::Network(int t_networkType, int t_mSF, int t_kWT, int t_betaWT, int t_probabilyConnectionER, int t_numberDomesticUnity)
  : indexLinkedDUs(t_numberDomesticUnity)
  , m_numberDomesticUnity(t_numberDomesticUnity)
  , m_mSF(t_mSF)
  , m_kWT(t_kWT)
  , m_betaWT(t_betaWT)
  , m_probabilyConnectionER(t_probabilyConnectionER)
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

// Create indexLinkedDUs based on a random network (Erdos-Renyi Model)
void Network::createRandomNetwork(){
  for(int i = 0; i < m_numberDomesticUnity; ++i){
    for(int j = i+1; j < m_numberDomesticUnity; ++j)
      if(uniFLOAT(rand64) < m_probabilyConnectionER){
        indexLinkedDUs[i].push_back(j);
        indexLinkedDUs[j].push_back(i);
      }
  }
}

// Create indexLinkedDUs based on a small-world network (Watts–Strogatz Model)
void Network::createWTNetwork(){

  // Create matrix and make ring network
  std::vector<int> matrix(m_numberDomesticUnity*m_numberDomesticUnity, 0);
  for(int lin = 0; lin < m_numberDomesticUnity; ++lin){
    for(int addTerm = 1; addTerm < 1 + m_kWT / 2; ++addTerm){
      int colL = lin - addTerm;
      int colR = lin + addTerm;
      if(colL < 0)
        colL = colL + m_numberDomesticUnity;
      if(colR > m_numberDomesticUnity - 1)
        colR = colR - m_numberDomesticUnity;
      if(colL > lin)
        matrix[lin*m_numberDomesticUnity+colL] = 1;
      if(colR > lin)
        matrix[lin*m_numberDomesticUnity+colR] = 1;
    }
  }

  // Remove links with probability betaWT
  int removedLinks = 0;
  std::uniform_int_distribution<long> uniIntDU(0,m_numberDomesticUnity-1);
  for(int i = 0; i < m_numberDomesticUnity*m_numberDomesticUnity; ++i){
    if(matrix[i] == 1 && uniFLOAT(rand64) < m_betaWT){
      matrix[i] = 0;
      ++removedLinks;
      }
  }

  // Rewire randomly
  for(int i = 0; i < removedLinks; ++i){
    int newDU1 = uniIntDU(rand64);
    int newDU2 = uniIntDU(rand64);
    while(newDU2 == newDU1 || matrix[newDU1*m_numberDomesticUnity+newDU2] == 1 || matrix[newDU2*m_numberDomesticUnity+newDU1] == 1)
      newDU2 = uniIntDU(rand64);
    matrix[newDU1*m_numberDomesticUnity+newDU2] = 1;
  }

  // Sum transpose
  for(int lin = 0; lin < m_numberDomesticUnity; ++lin){
    for(int col = lin + 1; col < m_numberDomesticUnity; ++col){
      matrix[lin*m_numberDomesticUnity+col] = matrix[lin*m_numberDomesticUnity+col] + matrix[col*m_numberDomesticUnity+lin];
      matrix[col*m_numberDomesticUnity+lin] = matrix[lin*m_numberDomesticUnity+col];
    }
  }

  // Fill indexLinkedDUs vector of vectors
  for(int i = 0; i < m_numberDomesticUnity; ++i){
    for(int j = 0; j < m_numberDomesticUnity; ++j)
      if(matrix[i*m_numberDomesticUnity+j] == 1)
        indexLinkedDUs[i].push_back(j);
  }

  // If there is an unnconnect DU, one additional connection is made
  for(int i = 0; i < m_numberDomesticUnity; ++i){
    if(indexLinkedDUs[i].empty()){
      int newDU = uniIntDU(rand64);
      indexLinkedDUs[i].push_back(newDU);
      indexLinkedDUs[newDU].push_back(i);
    }
  }
}

// Create indexLinkedDUs based on a scale-free network (Barabasi–Albert Model)
void Network::createSFNetwork(){
  int m0 = 2;
  int t = 47;
  std::vector<float> cumulative;
  indexLinkedDUs = std::vector<std::vector<int> >(m0);
  indexLinkedDUs[0].push_back(1);
  indexLinkedDUs[1].push_back(0);
  int lastNode = m0;
  for(int i = 0; i < t; ++i){
    cumulative = computeCumulativeDistribution();
    indexLinkedDUs.push_back(std::vector<int>(0));
    std::vector<int> newNodes;
    for(int j = 0; j < m_mSF; ++j){
      int node = 0;
      float rd = uniFLOAT(rand64);
      while(cumulative[node] < rd)
        ++node;
      if(std::find(newNodes.begin(), newNodes.end(), node) != newNodes.end())
        --j;
      else{
        indexLinkedDUs[lastNode].push_back(node);
        indexLinkedDUs[node].push_back(lastNode);
        newNodes.push_back(node);
      }
    }
    ++lastNode;
  }
}

// Compute the cumulative vector based on the degree of the node
std::vector<float> Network::computeCumulativeDistribution(void){
  std::vector<float> cumulative(indexLinkedDUs.size());
  cumulative[0] = indexLinkedDUs[0].size();
  for(int i = 1; i < indexLinkedDUs.size(); ++i)
    cumulative[i] = cumulative[i-1] + indexLinkedDUs[i].size();
  for(int i = 0; i < indexLinkedDUs.size(); ++i)
    cumulative[i] /= cumulative.back();
  cumulative.back() = 1.0;
  return cumulative;
}

void Network::printNetwork(void){
  for(int i = 0; i < indexLinkedDUs.size(); ++i){
    cout << "DU number " << i << ": ";
    for(auto j : indexLinkedDUs[i])
      cout << j << ", ";
    cout << endl;
  }
}

#endif
