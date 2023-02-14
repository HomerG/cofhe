#pragma once

#include <vector>
#include "numpy.hpp"
#include "tensorflow.hpp"

using std::vector;

template <class T> vector<vector<vector<vector<T>>>>
scaledMeanPool2d(const vector<vector<vector<vector<T>>>> & x, size_t kernel_size)
{
    auto no_of_samples = shape(x)[0];
    auto no_of_channels = shape(x)[3];
    auto sample_size = shape(x)[1];
    auto pool_size = kernel_size;
    auto strides = sample_size / pool_size;
    vector<vector<vector<vector<T>>>> out;
    for (size_t n=0; n<no_of_samples; n++)
    {
        vector<vector<vector<T>>> temp_per_channel(
            strides, vector<vector<T>>(
                strides, vector<T>(no_of_channels)
        ));
        for (size_t c=0; c<no_of_channels; c++)
        {
            for (size_t stride_row=0; stride_row<strides; stride_row++)
            {
                for (size_t stride_col=0; stride_col<strides; stride_col++)
                {
                    auto row_start = stride_row * pool_size;
                    auto col_start = stride_col * pool_size;
                    temp_per_channel[stride_row][stride_col][c] = mean( subvector(x, vector<size_t>{n,n+1, row_start,row_start+pool_size, col_start,col_start+pool_size, c,c+1}) );
                }
            }
        }
        out.push_back(temp_per_channel);
    }
    return out;
}

template <class T, class U> vector<size_t>
predict(const vector<vector<vector<vector<T>>>> & x, const vector<size_t> & y,
    const vector<vector<vector<vector<U>>>> & w1, const vector<T> & b1,
    const vector<vector<vector<vector<U>>>> & w4, const vector<T> & b4,
    const vector<vector<U>> & w8, const vector<T> & b8,
    double & acc)
{
    // cout << "is x encrypted? " << x[0][0][0][0].isEncrypted() << '\n';
    // cout << "Noise budget: " << x[0][0][0][0].noise() << '\n';
    auto h1 = add( conv2d(x, w1, vector<size_t>{1,2,2,1}, "SAME", "NHWC"), b1 );
    cout << "h1 "; print(shape(h1));
    // cout << "Noise budget: " << h1[0][0][0][0].noise() << '\n';
    auto h2 = square(h1);
    cout << "h2 "; print(shape(h2));
    // cout << "Noise budget: " << h2[0][0][0][0].noise() << '\n';
    auto h3 = scaledMeanPool2d(h2, 2);
    cout << "h3 "; print(shape(h3));
    // cout << "Noise budget: " << h3[0][0][0][0].noise() << '\n';
    auto h4 = add( conv2d(h3, w4,  vector<size_t>{1,2,2,1}, "SAME", "NHWC"), b4);
    cout << "h4 "; print(shape(h4));
    // cout << "Noise budget: " << h4[0][0][0][0].noise() << '\n';
    auto h5 = scaledMeanPool2d(h4, 2);
    cout << "h5 "; print(shape(h5));
    // cout << "Noise budget: " << h5[0][0][0][0].noise() << '\n';
    auto h6 = reshape2d( h5, vector<size_t>{h5.size(), h5[0].size() * h5[0][0].size() * h5[0][0][0].size()} );
    cout << "h6 "; print(shape(h6));
    // cout << "Noise budget: " << h6[0][0].noise() << '\n';
    auto h7 = square(h6);
    cout << "h7 "; print(shape(h7));
    // cout << "Noise budget: " << h7[0][0].noise() << '\n';
    auto h8 = add( dot(h7,w8), b8 );
    cout << "h8 "; print(shape(h8));
    // cout << "Noise budget: " << h8[0][0].noise() << '\n';
    auto h8_decoded = crtDecode(h8);
    cout << "h8_decoded "; print(shape(h8_decoded));
    auto h8_reshaped = reshapeOrder( h8_decoded, vector<size_t>{1,0,2} );
    cout << "h8_reshaped "; print(shape(h8_reshaped));
    auto h8_combined = combine(h8_reshaped);
    cout << "h8_combined "; print(shape(h8_combined));
    auto h8_reshaped2 = reshapeOrder( h8_combined, vector<size_t>{1,0} );
    cout << "h8_reshaped2 "; print(shape(h8_reshaped2));
    auto o = argmax(h8_reshaped2);
    cout << "o "; print(shape(o));
    acc = double( countEqual(o, y) ) / o.size();
    return o;
}
