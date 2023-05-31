#Assignment 4 directory

This directory contains source code and other files for Assignment 4.

Use this README document to store notes about design, testing, and
questions you have while developing your assignment.

Put:
    mutex()
    flock()
    unMutex()
    ...
    close()

Get:
    flock()
    ...
    close()


Worker_station:
    pop()
    handle()
    close()

dispatcher_station:
    listen()
    push()


These are all the basics code structures that I followed. The most difficult part was put() and getting everything locked properally.

I had the most success printing out the audit log of a test I was failing and seeing exactly what went wrong
and where, this let me think about how two threads could be returning 201.

I found that my problem was I was checking if a file existed outside of my mutex(), So two threads could be checking near the same
time before I created the file. So I moved my check to inside the mutex() and I had better success in my tests.