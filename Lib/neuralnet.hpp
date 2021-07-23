#pragma once
#include <vector>
#include "linalg.hpp"
#include <iostream>
#include <cmath>

class NeuralNetwork
{
    private:
    std::vector<int> m_Shapes;
    std::vector<Matrix<double>> m_W;
    std::vector<ColumnVector<double>> m_B;
    double m_eta;
    int m_batchSize;
    int m_epochLen;
    public:
    NeuralNetwork(std::vector<int>&& Shapes, double learning_rate = 0.1, int BatchSize = 10, int epochlen = 10):
        m_Shapes(Shapes), m_eta(learning_rate), m_batchSize(BatchSize), m_epochLen(epochlen)
    {
        for ( int i = 1; i < m_Shapes.size(); i++ )
            m_W.push_back(randMat(m_Shapes[i], m_Shapes[i - 1]));
        for ( int i = 1; i < m_Shapes.size(); i++ )
            m_B.push_back(ColumnVector<double>(randMat(m_Shapes[i], 1)));
    }

    ColumnVector<double> feedforward(ColumnVector<double> input)
    {
        assert(input.size() == m_Shapes[0]);
        for ( int i = 1; i < m_Shapes.size(); i++ )
            input = ColumnVector<double>((m_W[i - 1] * input) + m_B[i - 1]);

        return input;
    }

    std::pair<std::vector<Matrix<double>>, std::vector<ColumnVector<double>>> backprop(ColumnVector<double> input, const ColumnVector<double>& expectedOutput)
    {
        assert(input.size() == m_Shapes[0]);
        assert(expectedOutput.size() == m_Shapes.back());
        std::vector<ColumnVector<double>> xAct, yAct;

        yAct.push_back(input);

        std::vector<Matrix<double>> nablaW(m_W.size());
        std::vector<ColumnVector<double>> nablaB(m_B.size());

        for ( int i = 1; i < m_Shapes.size(); i++ )
        {
            input = ColumnVector<double>((m_W[i - 1] * input) + m_B[i - 1]);
            xAct.push_back(input);
            input = sigmoid(input);
            yAct.push_back(input);
        }

        ColumnVector<double> delta = yAct.back() - expectedOutput;

        for ( int i = ( int )nablaW.size() - 1; i >= 0; i-- )
        {
            yAct.pop_back();
            delta = hadamaradProduct(delta, sigmoid_prime(xAct.back())); // derivative with respect to x 

            nablaW[i] = (Matrix<double>&&)(delta * yAct.back().transpose());
            nablaB[i] = delta;
            delta = (xAct.back().transpose() * m_W[i]).transpose();
            xAct.pop_back();
        }

        return { nablaW, nablaB };
    }

    void SGD(std::vector<std::pair<ColumnVector<double>, int>> trainData,
        std::vector<std::pair<ColumnVector<double>, int>> testData)
    {
        if ( testData.size() )
            random_shuffle(testData.begin(), testData.end());

        std::vector<Matrix<double>> nablaW(m_W.size());
        std::vector<ColumnVector<double>> nablaB(m_B.size());

        for ( int i = 0; i <= m_W.size(); i++ )
            nablaW[i].setDim(m_W[i].rows(), m_W[i].cols()), nablaB[i].setDim(m_B[i].size());

        for ( int k = 0; k < m_epochLen; k++ )
        {
            // randomize the test data 
            random_shuffle(trainData.begin(), trainData.end());

            // iterate through the batches formed of the test data and compute the gradient decent for each batch
            for ( int i = 1; i + m_batchSize < trainData.size(); i += m_batchSize )
            {
                if ( i != 1 ) // reset nablaW and nablaB
                {
                    fillvectMat<double>(nablaW);
                    fillvectMat<double>(nablaB);
                }

                // compute the nablaW and nablaB based on the trainData from the current batch 
                for ( int j = i; j < i + m_batchSize; j++ )
                {
                    auto a = backprop(trainData[j].first, OneDim<double>(( int )m_Shapes.back(), ( int )trainData[j].second));
                    nablaW += a.first;
                    nablaB += a.second;
                }

                m_W -= m_eta * nablaW;
                m_B -= m_eta * nablaB;
            }

            if ( testData.size() )
            {
                int correct = 0;
                for ( int j = 0; j < testData.size(); j++ )
                    if ( MaxOutput(feedforward(testData[j].first)) == testData[j].second )
                        ++correct;
                std::cout << "Epoch " << k << " finished; Accuracy: " << ( double )correct / testData.size() << "\n";
            }
            else
                std::cout << "Epoch " << k << " finished;\n";
        }
    }

    int MaxOutput(const ColumnVector<double>& modelOutput)
    {
        double maxx = -1000000;
        int el = 0;

        for ( int i = 0; i < modelOutput.size(); i++ )
            if ( modelOutput[i] > maxx )
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

    ColumnVector<double> sigmoid(ColumnVector<double> x)
    {
        for ( int i = 0; i < ( int )x.size(); i++ )
            x[i] = sigmoid(x[i]);
        return x;
    }

    ColumnVector<double> sigmoid_prime(ColumnVector<double> x)
    {
        for ( int i = 0; i < ( int )x.size(); i++ )
            x[i] = sigmoid_prime(x[i]);
        return x;
    }
};
