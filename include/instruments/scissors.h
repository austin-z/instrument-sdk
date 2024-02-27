#ifndef INSTRUMENTS_SCISSORS_H
#define INSTRUMENTS_SCISSORS_H

#include <vector>
#include <cstdint>

class ScissorsImpl;

class Scissors
{
public:
    explicit Scissors(const char* serial_port_name);
    ~Scissors();

    Scissors(const Scissors &) = delete;
    Scissors(Scissors &&) = delete;
    void operator = (const Scissors &) = delete;
    void operator = (Scissors &&) = delete;

    /**
     * @brief Initializes the instrument.
     * 
     * This function initializes the instrument, ensuring it is ready for use.
     * 
     * @return true if initialization is successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Uninitializes the instrument.
     * 
     * This function performs the de-initialization of the instrument, including the destruction
     * of associated resources. After calling this function, the instrument should not be used.
     * 
     * @return true if uninitialization is successful, false otherwise.
     */
    bool uninitialize();

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
    void control(const std::vector<int16_t> &velocities);

private:
    ScissorsImpl* impl_;
};

#endif // INSTRUMENTS_SCISSORS_H
