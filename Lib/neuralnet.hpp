#pragma once
#include <vector>
#include "linalg.hpp"
#include <iostream>
#include <cmath>
#include "timer.hpp"

class NeuralNetwork
{
public:
	mine::vector<int> m_Shapes;
	mine::vector<Matrix<double>> m_W;
	mine::vector<ColumnVector<double>> m_B;
	double m_eta;
	int m_batchSize;
	int m_NBatches;
	int m_epochLen;
	int ImageperEpoch = 1000;

public:
	void init()
	{
		m_W.resize(m_Shapes.size() - 1);
		m_B.resize(m_Shapes.size() - 1);

		for (int i = 1; i < m_Shapes.size(); i++)
			m_W[i - 1] = ((Matrix<double>&&) randMat(m_Shapes[i], m_Shapes[i - 1]));
		
		for (int i = 1; i < m_Shapes.size(); i++)
			m_B[i - 1] = (Vector<double>&&) randVect(m_Shapes[i]);
	}

	NeuralNetwork(mine::vector<int>&& Shapes, double learning_rate = 0.15, int BatchSize = 32, int epochlen = 10, int Batches = 100) :
		m_Shapes(std::move(Shapes)), m_eta(learning_rate), m_batchSize(BatchSize), m_epochLen(epochlen), m_NBatches(Batches)
	{
		init();
	}

	NeuralNetwork(const mine::vector<int>& Shapes, double learning_rate = 0.15, int BatchSize = 32, int epochlen = 10, int Batches = 200) :
		m_Shapes(Shapes), m_eta(learning_rate), m_batchSize(BatchSize), m_epochLen(epochlen), m_NBatches(Batches)
	{
		init();
	}

	ColumnVector<double> feedforward(ColumnVector<double> input)
	{
		assert(input.size() == m_Shapes[0]);

		for (int i = 1; i < m_Shapes.size(); i++)
			input = (ColumnVector<double>&&)sigmoid(m_B[i - 1] + (ColumnVector<double>&&)(m_W[i - 1] * input));

		return input;
	}

	void backprop(ColumnVector<double> input, const ColumnVector<double>& expectedOutput,
		mine::vector<Matrix<double>>& nablaW, mine::vector<ColumnVector<double>>& nablaB, 
		mine::vector<ColumnVector<double>>& xAct, mine::vector<ColumnVector<double>>& yAct)
	{
		assert(input.size() == m_Shapes[0]);
		assert(expectedOutput.size() == m_Shapes.back());
		yAct[0] = input;

		for (int i = 1; i < m_Shapes.size(); i++)
		{
			input = (ColumnVector<double>&&)(m_B[i - 1] + (ColumnVector<double>&&)(m_W[i - 1] * input));
			xAct[i - 1] = input;
			sigmoid(input);
			yAct[i] = input;
		}

		int posY = yAct.size()-1, posX = xAct.size()-1;
		ColumnVector<double> delta = yAct[posY] - expectedOutput;

		for (int i = (int)nablaW.size() - 1; i >= 0; i--)
		{
			posY--;
			delta = (ColumnVector<double>&&) hadamaradProduct<double>(delta, sigmoid_prime(xAct[posX])); // derivative with respect to x 
			nablaW[i] += (Matrix<double>&&)(delta * RowVector<double>(yAct[posY]));
			nablaB[i] += delta;

			delta = (Vector<double>&&)((RowVector<double>(xAct[posX]) * m_W[i]));
			posX--;
		}
	}

	void SGD(mine::vector<std::pair<ColumnVector<double>, int>>& trainData,
		mine::vector<std::pair<ColumnVector<double>, int>>& testData)
	{
		int lastTest = 0, DataUsed = min(m_NBatches * m_batchSize, trainData.size());
		if (testData.size())
			mine::randomShuffle(testData);

		mine::vector<ColumnVector<double>> xAct, yAct;
		yAct.resize((int)m_Shapes.size());
		xAct.resize((int)(m_Shapes.size() - 1));

		mine::vector<Matrix<double>> nablaW(m_W.size());
		mine::vector<ColumnVector<double>> nablaB(m_B.size());

		assert(m_W.size() == m_B.size());

		for (int i = 0; i < m_W.size(); i++)
			nablaW[i].setDim(m_W[i].rows(), m_W[i].cols()), nablaB[i].setDim(m_B[i].size());

		for (int k = 0; k < m_epochLen; k++)
		{
			// randomize the test data 
			mine::randomShuffle(trainData);

			// iterate through the batches formed of the test data and compute the gradient decent for each batch
			for (int i = 1; i + m_batchSize <= DataUsed; i += m_batchSize)
			{
				fillvectMat<double>(nablaW);
				fillvectMat<double>(nablaB);

				int lim = min(i + m_batchSize, trainData.size());

				// compute the nablaW and nablaB based on the trainData from the current batch 
				for (int j = i; j < lim; j++)
					backprop(trainData[j].first, OneDim<double>((int)m_Shapes.back(), (int)trainData[j].second),
						nablaW, nablaB, xAct, yAct);

				m_W -= m_eta * nablaW;
				m_B -= m_eta * nablaB;
			}

			if (testData.size())
			{
				const int Len = 1000;
				int correct = 0;
				for (int j = lastTest; j < lastTest + Len; j++)
					if (MaxOutput(feedforward(testData[j].first)) == testData[j].second)
						++correct;
				std::cout << "Epoch " << k << " finished; Accuracy: " << (double)correct / Len << "\n";
				lastTest += Len;
				lastTest %= testData.size();
			}
			else
				std::cout << "Epoch " << k << " finished;\n";
		}
	}

	int MaxOutput(const ColumnVector<double>& modelOutput)
	{
		double maxx = -1000000;
		int el = 0;

		for (int i = 0; i < modelOutput.size(); i++)
			if (modelOutput[i] > maxx)
				el = i, maxx = modelOutput[i];
		return el;
	}

	double sigmoid(double x)
	{
		return (1.0 / (1 + 1.0 / expl(x)));
	}
	double sigmoid_prime(double x)
	{
		double sigm = sigmoid(x);
		return sigm * (1.0 - sigm);
	}

	void sigmoid(ColumnVector<double>& x)
	{
		for (int i = 0; i < (int)x.size(); i++)
			x[i] = sigmoid(x[i]);
	}

	ColumnVector<double> sigmoid_prime(ColumnVector<double> x)
	{
		for (int i = 0; i < (int)x.size(); i++)
			x[i] = sigmoid_prime(x[i]);
		return x;
	}

};
