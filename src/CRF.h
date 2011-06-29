#include <R.h>
#include <Rdefines.h>

/* Interfaces to R */

extern "C" {
	/* Decoding */
	SEXP Decode_Exact(SEXP _crf);
	SEXP Decode_Chain(SEXP _crf);
	SEXP Decode_Tree(SEXP _crf);
	SEXP Decode_Cutset(SEXP _crf);
	SEXP Decode_CutsetChain(SEXP _crf);
	SEXP Decode_LBP(SEXP _crf, SEXP _maxIter, SEXP _cutoff, SEXP _verbose);
	SEXP Decode_TRBP(SEXP _crf, SEXP _maxIter, SEXP _cutoff, SEXP _verbose);
	SEXP Decode_Sample(SEXP _crf, SEXP _samples);

	/* Inference */
	SEXP Infer_Exact(SEXP _crf);
	SEXP Infer_Chain(SEXP _crf);
	SEXP Infer_Tree(SEXP _crf);
	SEXP Infer_Cutset(SEXP _crf);
	SEXP Infer_CutsetChain(SEXP _crf);
	SEXP Infer_LBP(SEXP _crf, SEXP _maxIter, SEXP _cutoff, SEXP _verbose);
	SEXP Infer_TRBP(SEXP _crf, SEXP _maxIter, SEXP _cutoff, SEXP _verbose);
	SEXP Infer_Sample(SEXP _crf, SEXP _samples);

	/* Sampling */
	SEXP Sample_Exact(SEXP _crf, SEXP _size);
	SEXP Sample_Chain(SEXP _crf, SEXP _size);
	SEXP Sample_Tree(SEXP _crf, SEXP _size);
	SEXP Sample_Cutset(SEXP _crf, SEXP _size);
	SEXP Sample_CutsetChain(SEXP _crf, SEXP _size);
	SEXP Sample_Gibbs(SEXP _crf, SEXP _size, SEXP _burnIn, SEXP _start);

	/* Utils */
	SEXP Clamp_NodePot(SEXP _crfClamped);
	SEXP Get_Potential(SEXP _crf, SEXP _configuration);
	SEXP Get_LogPotential(SEXP _crf, SEXP _configuration);
}

/* CRF class */

class CRF {
public:
	SEXP _nNodes, _nEdges, _edges, _nStates, _maxState;
	int nNodes, nEdges, *edges, *nStates, maxState;

	SEXP _nAdj, _adjNodes, _adjEdges;
	int *nAdj, **adjNodes, **adjEdges;

	SEXP _nodePot, _edgePot;
	double *nodePot, *edgePot;

	SEXP _labels;
	int *labels;

	SEXP _nodeBel, _edgeBel, _logZ, _belief;
	double *nodeBel, *edgeBel, *logZ;

	SEXP _samples;
	int *samples, nSamples;

	int numProtect;

	CRF();
	CRF(SEXP _crf);
	~CRF();

	/* Initialize results */
	void Init_Labels();
	void Init_NodeBel();
	void Init_EdgeBel();
	void Init_LogZ();
	void Init_Belief();
	void Init_Samples(int size);
	void Init_Samples(SEXP _size);

	/* Set members */
	void Set_Data(SEXP _crf);
	void Set_Samples(SEXP _otherSamples);

	int Edges(int i, int j);
	double &NodePot(int n, int s);
	double &EdgePot(int s1, int s2, int e);
	double &NodeBel(int n, int s);
	double &EdgeBel(int s1, int s2, int e);
	int &Samples(int i, int n);

	/* Utils */
	double Get_Potential(int *configuration);
	double Get_LogPotential(int *configuration);

	/* BP functions */
	void TreeBP(double *messages_1, double *messages_2, bool maximize = false);
	void LoopyBP(double *messages_1, double *messages_2, int maxIter, double cutoff, int verbose, bool maximize = false);
	void Message2NodeBelief(double *messages_1, double *messages_2);
	void Message2EdgeBelief(double *messages_1, double *messages_2);
	void MaxOfMarginals();
	void BetheFreeEnergy();
	void TRBP(double *messages_1, double *messages_2, double *mu, double *scaleEdgePot, int maxIter, double cutoff, int verbose, bool maximize = false);
	void TRBP_Message2NodeBelief(double *messages_1, double *messages_2, double *mu);
	void TRBP_Message2EdgeBelief(double *messages_1, double *messages_2, double *mu, double *scaleEdgePot);
	void TRBP_BetheFreeEnergy(double *mu);
	void TRBP_Weights(double *mu);
	void TRBP_ScaleEdgePot(double *mu, double *scaleEdgePot);

	/* Decoding methods */
	void Decode_Exact();
	void Decode_Chain();
	void Decode_Tree();
	void Decode_LBP(int maxIter, double cutoff, int verbose);
	void Decode_TRBP(int maxIter, double cutoff, int verbose);
	void Decode_Sample();
	/* Inference methods */
	void Infer_Exact();
	void Infer_Chain();
	void Infer_Tree();
	void Infer_LBP(int maxIter, double cutoff, int verbose);
	void Infer_TRBP(int maxIter, double cutoff, int verbose);
	void Infer_Sample();
	/* Sampling methods */
	void Sample_Exact(int size = 0);
	void Sample_Chain(int size = 0);
	void Sample_Tree(int size = 0);
	void Sample_LBP(int maxIter, double cutoff, int verbose, int size = 0);
	void Sample_Gibbs(int burnIn, int *start, int size = 0);
};

class CRFclamped: public CRF {
public:
	SEXP _original;
	CRF original;

	SEXP _clamped, _nodeId, _nodeMap, _edgeId, _edgeMap;
	int *clamped, *nodeId, *nodeMap, *edgeId, *edgeMap;

	CRFclamped(SEXP _crf);

	void Reset_NodePot();

	/* Decoding methods */
	void Decode_Cutset();
	void Decode_CutsetChain();
	/* Inference methods */
	void Infer_Cutset();
	void Infer_CutsetChain();
	/* Sampling methods */
	void Sample_Cutset(int size = 0);
	void Sample_CutsetChain(int size = 0);
};

/* inline functions */

inline int CRF::Edges(int i, int j)
{
	return edges[i + nEdges * j];
}

inline double &CRF::NodePot(int n, int s)
{
	return nodePot[n + nNodes * s];
}

inline double &CRF::EdgePot(int s1, int s2, int e)
{
	return edgePot[s1 + maxState * (s2 + maxState * e)];
}

inline double &CRF::NodeBel(int n, int s)
{
	return nodeBel[n + nNodes * s];
}

inline double &CRF::EdgeBel(int s1, int s2, int e)
{
	return edgeBel[s1 + maxState * (s2 + maxState * e)];
}

inline int &CRF::Samples(int i, int n)
{
	return samples[i + nSamples * n];
}


/* initialize the list */
#define setValues(r, c, v) for (int i = 0; i < length(r); i++) c[i] = v

/* get/set the list element */
SEXP getListElement(SEXP list, const char *tag);
void setListElement(SEXP list, int i, const char *tag, SEXP value);

/* set dim of array */
void setDim2(SEXP array, int x1, int x2);
void setDim3(SEXP array, int x1, int x2, int x3);

/* sample from discret distribution */
int sample(int n, double *prob);
