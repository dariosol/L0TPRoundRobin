/*
 *  Created on: Mar 10, 2015
 *      Author: Marco Boretto marco.bore@gmail.com
 */

#pragma once
#ifndef MEPHEADER_H_
#define MEPHEADER_H_

#include <atomic>
#include <cstdint>
#include <boost/noncopyable.hpp>

#include "CustomMEPFragment.h"

namespace na62 {
class BrokenPacketReceivedError;
class UnknownSourceIDFound;
} /* namespace na62 */

namespace na62 {
namespace l0 {
class MEPFragment;
} /* namespace l0 */
} /* namespace na62 */

namespace na62 {
namespace l0 {

/**
 * Defines the structure of a L0 MEP header as defined in table 2 in NA62-11-02.
 */
struct MEP_HDR {
	// Number of L0 triggers since start of burst
	uint32_t firstEventNum :24;
	uint8_t sourceID;

	uint16_t mepLength; //  Total length of the MEP in bytes including the header
	uint8_t eventCount;
	uint8_t sourceSubID;
}__attribute__ ((__packed__));

class CustomMEP: private boost::noncopyable {
public:
	/**
	 * Reads the data coming from L0 and initializes the corresponding fields
	 */
	//CustomMEP(const char *data, const uint_fast16_t & dataLength);
	CustomMEP(int max_mep_number);

	/**
	 * Frees the data buffer (orignialData) that was created by the Receiver
	 *
	 * Should only be called by ~MEPFragment() as a MEP may not be deleted until every MEPFragment is processed and deleted.
	 */
	virtual ~CustomMEP();

	void initializeMEPFragments(const char* data, const uint_fast16_t& dataLength);

	/**
	 * Returns a pointer to the n'th event within this MEP where 0<=n<getFirstEventNum()
	 */
	inline CustomMEPFragment* getFragment(const uint_fast16_t n) {
		/*
		 * n may be bigger than <getNumberOfEvents()> as <deleteEvent()> could have been invoked already
		 */
		return fragments_[n];
	}

	/**
	 * Returns the source ID of the detector that has sent this MEP
	 */
	inline uint_fast8_t getSourceID() const {
		return rawData_->sourceID;
	}

	/**
	 * Returns the event number of the first event fragment
	 */
	inline uint_fast32_t getFirstEventNum() const {
		return rawData_->firstEventNum;
	}

	/**
	 * Returns the number of MEP event fragments stored in this MEP
	 */
	inline uint_fast16_t getNumberOfFragments() const {
		return rawData_->eventCount;
	}

	/**
	 * Total length of the MEP in bytes including the header
	 */
	inline uint_fast16_t getLength() const {
		return rawData_->mepLength;
	}

	/**
	 * Returns the ID of the read out board as provided by the MEP frame header
	 */
	inline uint_fast8_t getSourceSubID() const {
		return rawData_->sourceSubID;
	}

	inline MEP_HDR const* getRawMepData() const {
		return rawData_;
	}

	inline bool isLastBurstPacket() {
		return is_last_burst_packet_;
	}

private:
	std::atomic<int> eventCount_;
	// Pointer to the payload of the UDP packet
	const MEP_HDR* rawData_;
        bool is_last_burst_packet_;
	CustomMEPFragment **fragments_;
};

} /* namespace l2 */
} /* namespace na62 */
#endif /* MEPHEADER_H_ */
