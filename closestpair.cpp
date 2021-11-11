#include "closestpair.h"
#include <algorithm>
#include <cstdlib>
#include <sys/time.h>  // used to seed srand for extra credit
#include <cmath>
#include <iostream>
using namespace std;

Outcome brute(const vector<Point>& data) {
    Outcome best(data[0], data[1], distSquared(data[0],data[1]));
    for (unsigned int j=1; j < data.size(); j++) {
        for (unsigned int k=0; k < j; k++) {
            long long temp = distSquared(data[j], data[k]);
            if (temp < best.dsq) {
                best = Outcome(data[j], data[k], temp);
            }
        }
    }
    return best;
}

Outcome outcomeMin (Outcome a, Outcome b){
    return (a.dsq < b.dsq)? a : b;
}

Outcome efficient_helper(const vector<Point>& xSorted, long xStart, long xEnd, vector<Point>& ySorted, long yStart){
    long size = xEnd - xStart;

    long yEnd = yStart + size;
    
    long xMid = xStart + size/2;
    long centerLine = xSorted[xMid].x;

    //cout << xStart << " " << xEnd << " " << xMid << " " << centerLine << " " << yStart << " " << yEnd << " " << endl;
    //use brute force if small enough data size
    if (size <= 100) {
        vector<Point> data;
        for (long k=xStart; k < xEnd; k++)
            data.push_back(xSorted[k]);
        return brute(data);
    }


    //linear pass through y to add all the left side to end of vector
    //then recursive call to solve left side
    for (long k=yStart; k < yEnd; k++){
        long i = yEnd;
        if (ySorted[k].x < centerLine){
            ySorted[i] = ySorted[k];
            i++;
        }
    }
    Outcome deltaLeft = efficient_helper(xSorted, xStart, xMid, ySorted, yEnd);

    //linear pass through y to add all the right side to end of vector
    //then recursive call to solve right side
    for (long k = yStart; k < yEnd; k++){
        long i = yEnd;
        if (ySorted[k].x >= centerLine){
            ySorted[i] = ySorted[k];
            i++;
        }
    }
    Outcome deltaRight = efficient_helper(xSorted, xMid, xEnd, ySorted, yEnd);

    Outcome delta = outcomeMin(deltaLeft, deltaRight);

    //linear pass through y to add all points in center strip to new vector
    vector<Point> pointsInStrip;
    for (long k=yStart; k < yEnd; k++){
        if(pow((ySorted[k].x - centerLine),2) < delta.dsq){
            pointsInStrip.push_back(ySorted[k]);
        }
    }

    //pass through points in center strip
    for (long k=1; k < pointsInStrip.size(); k++){
            //starting at the second point in center strip check points above and 
            //see if y dist^2 is less than delta if so then check if those two
            //points delta is less than current delta and make new delta
        long i = k-1;
        while (pow((pointsInStrip[k].y - pointsInStrip[i].y), 2) < delta.dsq){
            if(i == -1) break;
            delta = outcomeMin(delta, Outcome(pointsInStrip[k], pointsInStrip[i]));
            i--;
        }
    }
    return delta;
}

Outcome efficient(const vector<Point>& data) {
    vector<Point> ySorted, xSorted;
    long dataSize = data.size();

    xSorted = data;   
    sort(xSorted.begin(), xSorted.end(), compareByX);
    
    ySorted.resize(2*dataSize);
    ySorted = data;
    sort(ySorted.begin(), ySorted.end(), compareByY);

    Outcome closest = efficient_helper(xSorted, 0, dataSize, ySorted, 0);
    return closest;
}

Outcome extra(const vector<Point>& data) {
    return Outcome();
}