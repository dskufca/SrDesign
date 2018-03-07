// gps_l2_l_dll_pll_tracking.cc
// Danielle Skufca
// based on gps_l2_m_dll_pll_tracking.cc
// Implementation of an adapter of a DLL+PLL tracking loop block
// for GPS L2CL to a tracking interface

#include "gps_l2_l_dll_pll_tracking.h"
#include <glog/logging.h>
#include "GPS_L2C.h" 
#include "configuration_interface.h"

using google::LogMessage;

GpsL2LDllPllTracking::GpsL2LDllPllTracking(
		ConfigurationInterface* configuration, std::string role,
		unsigned int in_streams, unsigned int out_streams) :
				role_(role), in_streams_(in_streams), out_streams_(out_streams)
{
	DLOG(INFO) << "role " << role;
	//###################### CONFIGURATION PARAMETERS #####################
	int fs_in;
	int vector_length;
	int f_if;
	bool dump;
	std::string dump_filename;
	std::string item_type;
	std::string default_item_type = "gr_complex";
	float pll_bw_hz;
	float dll_bw_hz;
	float early_late_space_chips;
    item_type = configuration->property(role + ".item_type", default_item_type);
    fs_in = configuration->property("GNSS-SDR.internal_fs_hz", 2048000);
    f_if = configuration->property(role + ".if", 0);
    dump = configuration->property(role + ".dump", false);
    pll_bw_hz = configuration->property(role + ".pll_bw_hz", 50.0);
    dll_bw_hz = configuration->property(role + ".dll_bw_hz", 2.0);
    early_late_space_chips = configuration->property(role + ".early_late_space_chips", 0.5);
    std::string default_dump_filename = "./track_ch";
    dump_filename = configuration->property(role + ".dump_filename",
            default_dump_filename); //unused!
    vector_length = std::round(fs_in / (GPS_L2_L_CODE_RATE_HZ / GPS_L2_L_CODE_LENGTH_CHIPS));

    //################# MAKE TRACKING GNURadio object ###################
    if (item_type.compare("gr_complex") == 0)
        {
            item_size_ = sizeof(gr_complex);
            tracking_ = gps_l2_l_dll_pll_make_tracking_cc(
                    f_if,
                    fs_in,
                    vector_length,
                    dump,
                    dump_filename,
                    pll_bw_hz,
                    dll_bw_hz,
                    early_late_space_chips);
        }
    else
        {
            item_size_ = sizeof(gr_complex);
            LOG(WARNING) << item_type << " unknown tracking item type.";
        }
    channel_ = 0;
    DLOG(INFO) << "tracking(" << tracking_->unique_id() << ")";
}


GpsL2LDllPllTracking::~GpsL2LDllPllTracking()
{}


void GpsL2LDllPllTracking::start_tracking()
{
    tracking_->start_tracking();
}

/*
 * Set tracking channel unique ID
 */
void GpsL2LDllPllTracking::set_channel(unsigned int channel)
{
    channel_ = channel;
    tracking_->set_channel(channel);
}


void GpsL2LDllPllTracking::set_gnss_synchro(Gnss_Synchro* p_gnss_synchro)
{
    tracking_->set_gnss_synchro(p_gnss_synchro);
}

void GpsL2LDllPllTracking::connect(gr::top_block_sptr top_block)
{
    if(top_block) { /* top_block is not null */};
    //nothing to connect, now the tracking uses gr_sync_decimator
}

void GpsL2LDllPllTracking::disconnect(gr::top_block_sptr top_block)
{
    if(top_block) { /* top_block is not null */};
    //nothing to disconnect, now the tracking uses gr_sync_decimator
}

gr::basic_block_sptr GpsL2LDllPllTracking::get_left_block()
{
    return tracking_;
}

gr::basic_block_sptr GpsL2LDllPllTracking::get_right_block()
{
    return tracking_;
}

