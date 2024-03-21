#ifndef ABSTRACT_INSTRUMENTS_H
#define ABSTRACT_INSTRUMENTS_H

#include <vector>
#include <cstdint>

class AbstractInstrument
{
public:
    AbstractInstrument() = default;
    virtual ~AbstractInstrument() = default;

    AbstractInstrument(const AbstractInstrument &) = delete;
    AbstractInstrument(AbstractInstrument &&) = delete;
    void operator = (const AbstractInstrument &) = delete;
    void operator = (AbstractInstrument &&) = delete;

    /**
     * @brief Initializes the instrument.
     * 
     * This function initializes the instrument, ensuring it is ready for use.
     * 
     * @return true if initialization is successful, false otherwise.
     */
    virtual bool initialize() = 0;

    /**
     * @brief Uninitializes the instrument.
     * 
     * This function performs the de-initialization of the instrument, including the destruction
     * of associated resources. After calling this function, the instrument should not be used.
     * 
     * @return true if uninitialization is successful, false otherwise.
     */
    virtual bool uninitialize() = 0;

    /**
     * @brief Controls the motion of the instrument.
     *
     * This function controls the motion of the instrument.
     * It adjusts the velocity of each DOF according to the provided vector.
     * Each time the function is called, the control lasts for 50 ms.
     * If the function is called again within 50 ms, the new control command will be executed.
     *
     * @param velocities A vector containing the motion speeds for each DOF.
     */
    virtual void control(const std::vector<int16_t> &velocities) = 0;
};

#endif // ABSTRACT_INSTRUMENTS_H
