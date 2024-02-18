#ifndef INSTRUMENTS_FORCEPS_H
#define INSTRUMENTS_FORCEPS_H

#include <vector>
#include <cstdint>

class ForcepsImpl;

class Forceps
{
public:
    explicit Forceps(const char* serial_port_name);
    ~Forceps();

    /**
     * @brief Initializes the instrument.
     * 
     * This function initializes the instrument, ensuring it is ready for use.
     * 
     * @return true if initialization is successful, false otherwise.
     */
    [[nodiscard]]
    bool initialize();

    /**
     * @brief Uninitializes the instrument.
     * 
     * This function performs the de-initialization of the instrument, including the destruction
     * of associated resources. After calling this function, the instrument should not be used.
     * 
     * @return true if uninitialization is successful, false otherwise.
     */
    [[nodiscard]]
    bool uninitialize();

    /**
     * @brief Controls the motion of the instrument.
     *
     * This function controls the motion of the instrument.
     * It adjusts the velocity of each DOF according to the provided vector.
     *
     * @param velocities A vector containing the motion speeds for each DOF.
     */
    void control(const std::vector<int16_t> &velocities);

private:
    ForcepsImpl* impl_;
};

#endif
