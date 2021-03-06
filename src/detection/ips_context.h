//--------------------------------------------------------------------------
// Copyright (C) 2016-2018 Cisco and/or its affiliates. All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License Version 2 as published
// by the Free Software Foundation.  You may not use, modify or distribute
// this program under any other version of the GNU General Public License.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//--------------------------------------------------------------------------

// ips_context.h author Russ Combs <rucombs@cisco.com>

#ifndef IPS_CONTEXT_H
#define IPS_CONTEXT_H

// IpsContext provides access to all the state required for detection of a
// single packet.  the state is stored in IpsContextData instances, which
// are accessed by id.

#include "main/snort_types.h"
#include "framework/codec.h"

// required to get a decent decl of pkth
#include "protocols/packet.h"

#include "detection/detection_util.h"

class SO_PUBLIC IpsContextData
{
public:
    virtual ~IpsContextData() = default;

    static unsigned get_ips_id();
    static unsigned get_max_id();

protected:
    IpsContextData() = default;
};

class SO_PUBLIC IpsContext
{
public:
    IpsContext(unsigned size = 0);  // defaults to max id
    ~IpsContext();

    IpsContext(const IpsContext&) = delete;
    IpsContext& operator=(const IpsContext&) = delete;

    void set_context_data(unsigned id, IpsContextData*);
    IpsContextData* get_context_data(unsigned id) const;

    void set_slot(unsigned s)
    { slot = s; }

    unsigned get_slot()
    { return slot; }

    enum ActiveRules
    { NONE, NON_CONTENT, CONTENT };

public:
    Packet* packet;
    Packet* encode_packet;
    DAQ_PktHdr_t* pkth;
    uint8_t* buf;

    struct SnortConfig* conf;
    class MpseStash* stash;
    struct OtnxMatchData* otnx;
    struct SF_EVENTQ* equeue;

    DataPointer file_data;
    DataBuffer alt_data;

    uint64_t context_num;
    ActiveRules active_rules;
    bool check_tags;

    static const unsigned buf_size = Codec::PKT_MAX;

private:
    std::vector<IpsContextData*> data;
    unsigned slot;
};

#endif

