// gps_l2_l_dll_pll_tracking.h
// Danielle Skufca
// Sr Design 2018

// This is a header file for a class that implements a code DLL
// and carrier PLL tracking loop 

#ifndef GNSS_SDR_gps_l2_l_dll_pll_tracking_H_
#define GNSS_SDR_gps_l2_l_dll_pll_tracking_H_

#include <string>
#include "tracking_interface.h"
#include "gps_l2_l_dll_pll_tracking_cc.h"

class ConfigurationInterface;

/*!
 * \brief This class implements a code DLL and carrier PLL tracking loop for L2CL
 */ 
class GpsL2LDllPllTracking : public TrackingInterface
{
public:
    GpsL2LDllPllTracking(ConfigurationInterface* configuration,
            std::string role,
            unsigned int in_streams,
            unsigned int out_streams);

	virtual ~GpsL2LDllPllTracking();

	std::string role()
	{
		return role_;
	}

	//! Returns "GPS_L2_L_DLL_PLL_Tracking"
	std::string implementation()
	{
		return "GPS_L2_L_DLL_PLL_Tracking";
	}

	size_t item_size()
	{
		return item_size_;
	}

	void connect(gr::top_block_sptr top_block);
	void disconnect(gr::top_block_sptr top_block);
	gr::basic_block_sptr get_left_block();
	gr::basic_block_sptr get_right_block();

	/*! 
	 * \brief Set tracking channel unique ID
	 */
	void set_channel(unsigned int channel);

	/*!
	 * \brief Set acquisition/tracking common Gnss_Synchro object pointer
	 * to efficiently exchange synchronization data between acquision and tracking blocks
	 */
	void set_gnss_synchro(Gnss_Synchro* p_gnss_synchro);

	void start_tracking();

private:
	gps_l2_l_dll_pll_tracking_cc_sptr tracking_;
	size_t item_size_;
	unsigned int channel_;
	std::string role_;
	unsigned int in_streams_;
	unsigned int out_streams_;
};

#endif 
