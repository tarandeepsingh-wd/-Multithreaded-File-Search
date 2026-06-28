Multithreaded File Search

This project implements a multithreaded file search utility that efficiently searches for a given keyword across multiple files.
The program supports both .txt and .pdf file formats and is designed to demonstrate practical use of multithreading, concurrency, and file I/O in a systems programming context.

The core idea is to improve search performance by parallelizing file scanning. Each file (or group of files) is processed by a separate thread, 
allowing multiple files to be searched simultaneously. This reduces overall execution time compared to a single-threaded approach, especially when working with large directories or many files.

To ensure correctness during concurrent execution, synchronization mechanisms are used to safely manage shared resources such as result counters and
output structures. This prevents race conditions and ensures consistent results across threads.

The program follows a modular design, separating file discovery, thread management, and content scanning logic. It efficiently reads file contents,
searches for the target word, and reports matches in a clear and structured manner. For PDF files, text extraction is handled before performing the search.

This project demonstrates key operating system concepts including thread creation, workload distribution, synchronization, and performance optimization
through parallelism. It also highlights good practices in concurrent programming such as safe shared-state management and clean thread lifecycle handling.
