#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

#define LL long long
#define micro_duration std::chrono::duration_cast<std::chrono::seconds>
#define time_now std::chrono::high_resolution_clock::now()

LL int globalSum;
std::mutex globalSumMutex;

LL int getOddSum(const LL int& size){
    LL int sum = 0;
    for(int i = 1; i <= size; i+=2){
        sum+=i;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return sum;
}

LL int getEvenSum(const LL int& size){
    LL int sum = 0;
    for(int i = 0; i <= size; i+=2){
        sum+=i;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return sum;
}

void addOdd(const LL int& size){
    LL int sum = getOddSum(size);
    globalSumMutex.lock();
    // critical section
    globalSum += sum;       
    globalSumMutex.unlock();
}

void addEven(const LL int& size){
    LL int sum = getEvenSum(size);
    globalSumMutex.lock();
    // critical section
    globalSum += sum;       
    globalSumMutex.unlock();
}


int main(){
    LL int N = 100000;
    LL int sequential_sum = 0;
    std::cout << "Starting Sequential Calculation:" << std::endl;
    auto start = time_now;
    sequential_sum += getOddSum(N);
    sequential_sum += getEvenSum(N);
    auto end = time_now;
    auto duration = micro_duration(end-start);
    std::cout << "Sequential Sum: " << sequential_sum << " and time: " << duration.count() << "s" << std::endl;
    
    
    std::cout << "\nStarting Thread Calculation:" << std::endl;
    start = time_now;
    std::thread t1(addOdd, N);
    std::thread t2(addEven, N);
    t1.join();
    t2.join();
    end = time_now;
    duration = micro_duration(end-start);
    std::cout << "Thread Sum: " << globalSum << " and time: " << duration.count() << "s" << std::endl;
    
    
    LL int sm = (N/2)*(N+1);
    std::cout << "\nCorrect Answer: " << sm << std::endl;

    std::cout << "\nConclusion: " << std::endl;
    std::cout << "- In a sequential approach, you make one API call and wait for each call to complete before moving on to the next. This can be simple to implement but can lead to inefficient resource usage, especially if the API calls have significant latency or processing time."
    << "\n- You can use threads to execute multiple API calls concurrently, allowing them to run in parallel and potentially reducing the overall execution time. Each API call can be performed in a separate thread, and the results can be collected when all threads have finished." 
    << std::endl;
    return 0;
}