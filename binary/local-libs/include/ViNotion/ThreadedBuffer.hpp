/**\file
 * ThreadedBuffer class implementation.
 *
 * $Id: ThreadedBuffer.hpp 1758 2010-09-27 11:24:37Z rick $
 */

#ifndef THREADED_BUFFER_HPP
#define THREADED_BUFFER_HPP

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <deque>
#include <iostream>

#define VIDEBUG 0

namespace Vi
{
  /// Threadsave buffer. It is a wrapper around a std::deque.
  template<class T>
  class ThreadedBuffer
  {
  public:
    /**
     * Constructor.
     *
     * \param[in] fBufSize   Maximum size of buffer.
     * \param[in] fDropItems True to drop items when the buffer is full, instead of blocking. 
     */
    ThreadedBuffer(unsigned int fBufSize = 100, bool fDropItems = false);

    /**
     * Put an item into the queue.
     */
    void put(T m);

    /**
     * Get an item from the queue.
     */
    T get();

    /**
     * Set drop items.
     *
     * \param[in] fDropItems True to drop items when the buffer is full, instead of blocking. 
     */
    void setDropItems(bool fDropItems);

    /**
     * Set buffer size.
     *
     * \param[in] fBufSize Maximum size of buffer.
     */
    void setBufSize(unsigned int fBufSize);

    /**
     * Get current number of elements in the buffer.
     *
     * \return Number of elements.
     */
    unsigned int getNumElements();

  private:
    boost::mutex mMutex;              ///< Mutex to control deque access.
    boost::condition_variable mCond;  ///< Conditional to wait for empty/full deque.
    unsigned int mBufSize;            ///< Maximum allowable buffer size.
    std::deque<T> mBuf;               ///< The actual buffer.
    bool mDropItems;                  ///< True to drop items when the buffer is full, instead of blocking.
  };


  template<class T>
  unsigned int ThreadedBuffer<T>::getNumElements()
  {
    boost::unique_lock<boost::mutex> lock(mMutex);
    return mBuf.size();
  }

  template<class T>
  ThreadedBuffer<T>::ThreadedBuffer(unsigned int fBufSize, bool fDropItems) :
    mBufSize(fBufSize),
    mDropItems(fDropItems)
  {}

  template<class T>
  void ThreadedBuffer<T>::setDropItems(bool fDropItems)
  {
    boost::unique_lock<boost::mutex> lock(mMutex);
    mDropItems = fDropItems;
  }

  template<class T>
  void ThreadedBuffer<T>::setBufSize(unsigned int fBufSize)
  {
    boost::unique_lock<boost::mutex> lock(mMutex);
    mBufSize = fBufSize;
  }

  template<class T>
  void ThreadedBuffer<T>::put(T m)
  {
    boost::unique_lock<boost::mutex> lock(mMutex);

    if (mBuf.size() == mBufSize)
    { 
      if (mDropItems)
      {
        // Drop item at front of queue.
#ifdef DEBUG_THREADED_BUFFER
        std::cout << "ThreadedBuffer<T>::put: Dropping item: " << mBuf.front() << "." << std::endl;        
#endif
        mBuf.pop_front();
      }
      else
      {
        // Block until a slot is freed.
        if (VIDEBUG)         
          std::cout << "ThreadedBuffer<T>::put: Buffer is full." << std::endl;
        
        while (mBuf.size() == mBufSize) mCond.wait(lock);
      }     
    }

#ifdef DEBUG_THREADED_BUFFER
    std::cout << "ThreadedBuffer<T>::put: " << m << std::endl;
#endif

    mBuf.push_back(m);
    mCond.notify_one();
  }

  template<class T>
  T ThreadedBuffer<T>::get()
  {
    boost::unique_lock<boost::mutex> lock(mMutex);

    if (mBuf.size() == 0 && VIDEBUG)
      std::cout << "ThreadedBuffer<T>::get: Buffer is empty. Waiting..." << std::endl;

    while (mBuf.size() == 0) mCond.wait(lock);

    T tmp = mBuf.front();
    mBuf.pop_front();
    mCond.notify_one();

#ifdef DEBUG_THREADED_BUFFER
    std::cout << "ThreadedBuffer<T>::get: " << tmp << std::endl;
#endif

    return tmp;
  }
}
#endif // THREADED_BUFFER_HPP
