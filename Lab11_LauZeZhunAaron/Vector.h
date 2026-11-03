#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/**
 * @file Vector.h
 * @author 35362795 Lau Ze Zhun Aaron
 * @version 1.0
 * @date 04/10/2026
 * @brief Declaration and implementation of the Vector template class
 */

/**
 * @class Vector
 * @brief Stores objects in an encapsulated dynamic array
 * @tparam T Element type that supports the operations used by this Vector
 *
 * This is a Vector Template
 * The array grows before insertion when more than half its capacity is used
 */
template <class T>
class Vector
{
public:
    /**
     * @brief Creates an empty Vector
     */
    Vector();

    /**
     * @brief Creates a Vector with an initial number of stored elements
     * @param n Initial size and capacity when positive
     * @note Assign numeric elements before reading them
     * @note A nonpositive n creates an empty Vector with capacity one
     */
    Vector(int n);

    /**
     * @brief Creates a deep copy of another Vector
     * @param other Vector to copy
     */
    Vector(const Vector<T> & other);

    /**
     * @brief Releases the dynamic array
     */
    ~Vector();

    /**
     * @brief Makes a deep copy of another Vector
     * @param other Vector to copy
     * @return Reference to this Vector
     */
    Vector<T> & operator = (const Vector<T> & other);

    /**
     * @brief Provides access to an element
     * @param index Position of the element
     * @return Reference to the selected element
     * @pre index is between 0 and Size() - 1
     * @note The caller must check the index because this operator has no bounds check
     */
    T & operator [] (int index);

    /**
     * @brief Provides read-only access to an element
     * @param index Position of the element
     * @return Constant reference to the selected element
     * @pre index is between 0 and Size() - 1
     * @note The caller must check the index because this operator has no bounds check
     */
    const T & operator [] (int index) const;

    /**
     * @brief Inserts an element at a selected position
     * @param pos Position from 0 to Size(), inclusive
     * @param data Element to insert
     * @return True if inserted; false if pos is invalid or there is no space
     * @note Position Size() appends; earlier positions shift elements right
     */
    bool Insert(int pos, const T & data);

    /**
     * @brief Removes an element and shifts following elements left
     * @param pos Position of the element to remove
     * @return True if removed; false if pos is outside 0 to Size() - 1
     * @note The allocated capacity stays unchanged
     */
    bool Delete(int pos);

    /**
     * @brief Retrieves the number of stored elements
     * @return Number of stored elements
     */
    int Size() const;

private:
    T * m_data; ///< Points to the dynamic array owned by this Vector
    int m_size; ///< Number of logically stored elements, not the allocated capacity
    int m_capacity; ///< Number of allocated array slots

    /**
     * @brief Doubles capacity while preserving stored elements
     */
    void Resize();

    /**
     * @brief Shifts elements right to make an insertion gap
     * @param pos Valid insertion position
     * @pre There is space for one extra element
     */
    void ShiftRight(int pos);

    /**
     * @brief Shifts elements left to close a deletion gap
     * @param pos Position of an existing element
     */
    void ShiftLeft(int pos);

    /**
     * @brief Replaces the owned array with an independent copy
     * @param other Vector whose elements are copied
     * @pre m_data is zero or points to the array owned by this Vector
     */
    void Copy(const Vector<T> & other);
};

// Template implementation is kept below the interface in the same header file

template <class T>
Vector<T>::Vector() //Default constructor
{
    m_size = 0;
    m_capacity = 10;
    m_data = new T[m_capacity]; // Creates the first block of heap memory
}

template <class T>
Vector<T>::Vector(int n) //Constructor with a size
{
    if(n > 0)
    {
        m_size = n;
        m_capacity = n;
    }
    else
    {
        m_size = 0;
        m_capacity = 1;
    }

    m_data = new T[m_capacity]; // Numeric elements must be assigned before being read
}

template <class T>
Vector<T>::Vector(const Vector<T> & other) //Copy constructor
{
    m_data = 0; // There is no old array to release during construction
    Copy(other);
}

template <class T>
Vector<T>::~Vector() //Destructor
{
    delete [] m_data; // Prevents the dynamic array from leaking
}

//It checks for self-assignment, then uses Copy to make a deep copy so the two Vectors don’t share the same array
//It returns the current Vector by reference
template <class T>
Vector<T> & Vector<T>::operator = (const Vector<T> & other) //Assignment operator eg second = first,
{
    if(this != &other) // Avoids deleting data during self-assignment or The check makes sure they are different objects, avoiding unnecessary copying for:
    {
        Copy(other); // The helper creates independent storage and releases the old array
    }

    return *this;
}

//The non-const square-bracket operator returns a reference to the stored element, allowing it to be read or modified
template <class T>
T & Vector<T>::operator [] (int index) //Two array-access operators , This lets a non-const Vector use array-style access:
{
    return m_data[index]; // Returns the element at an index that the caller must keep within bounds
}

//const T & — prevents modifying the returned element through that reference
//The final const — promises the method won’t modify the Vector’s ordinary member variables
template <class T>
const T & Vector<T>::operator [] (int index) const //Const array-access operator. This is used when the Vector is accessed as const, such as processing parameter: const WeatherRecords & data
{
    return m_data[index];
}


//0 <= pos <= m_size
//pos < 0 — before the beginning of the Vector
//pos > m_size — beyond the next available logical position, which would leave a gap

template <class T>
bool Vector<T>::Insert(int pos, const T & data)
{
    if(pos < 0 || pos > m_size) //Checks whether the position is outside the allowed insertion range: 0 through m_size
    {
        return false; // Insertion may use size because that position appends
    }

    T savedData = data; // Keeps the value safe if it comes from this Vector, preserving the value if any resizing
    if(m_size > m_capacity / 2)
    {
        Resize(); // Grows the array when more than half its capacity is used
    }

    if(m_size < m_capacity) //Checks whether the array has room for one more element
    {
        ShiftRight(pos); //Make space at the position
        m_data[pos] = savedData; //Store the element
        m_size++; //Update the count
        return true;
    }
    return false;
}
template <class T>
bool Vector<T>::Delete(int pos) //
{
    if(pos < 0 || pos >= m_size) //m_size is the count, not the last index.  there is no fourth stored value at index 3 to remove
    {
        return false; // An empty Vector also has no valid deletion position
    }
    ShiftLeft(pos);
    m_size--; // Only the elements before the new size belong to the Vector
    return true;
}

template <class T>
int Vector<T>::Size() const
{
    return m_size;
}

template <class T>
void Vector<T>::Resize()
{
    int newCapacity = m_capacity * 2; //new capacity, 10 becomes 20, 20 becomes 40 etc etc
    T * newData = new T[newCapacity]; //new array

    for(int i = 0; i < m_size; i++) //Loops through the existing stored elements
    {
        newData[i] = m_data[i]; // Copies existing values into the larger array
    }

    delete [] m_data; //Releases the old array, after its elements have been copied
    m_data = newData; //Makes the Vector’s pointer refer to the new array
    m_capacity = newCapacity; //Updates the recorded capacity
}

template <class T>
void Vector<T>::ShiftRight(int pos) //copy backwards
{
    for(int i = m_size; i > pos; i--)
    {
        m_data[i] = m_data[i - 1]; // Start at the end so existing elements are not overwritten, eg m_data[3] = m_data[2]; // Copy 30 to index 3 ,,,, m_data[2] = m_data[1]; // Copy 20 to index 2
    }
}

template <class T>
void Vector<T>::ShiftLeft(int pos)  //copy forwards,
{
    for(int i = pos; i < m_size - 1; i++)
    {
        m_data[i] = m_data[i + 1]; // Fill the gap using the next element. eg m_data[1] = m_data[2]; // Copy 20 to index 1 ,,,,  m_data[2] = m_data[3]; // Copy 30 to index 2
    }
}

template <class T>
void Vector<T>::Copy(const Vector<T> & other) //deep copy helper, copy constructor and assignment operator both call it
{
    T * newData = new T[other.m_capacity]; // A new array prevents shared ownership , Allocates a separate array with the same capacity as the source
    for(int i = 0; i < other.m_size; i++) //Loops through the source’s stored elements, not its unused slots
    {
        newData[i] = other.m_data[i];
    }
    delete [] m_data; // Deleting zero is safe when called by the copy constructor
    m_data = newData; //Makes the current Vector point to its new array
    m_size = other.m_size; //Copies the source’s stored count
    m_capacity = other.m_capacity; //Copies the source’s allocated capacity
}

#endif // VECTOR_H_INCLUDED
