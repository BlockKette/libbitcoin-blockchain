/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_BLOCKCHAIN_VALIDATE_BLOCK_HPP
#define LIBBITCOIN_BLOCKCHAIN_VALIDATE_BLOCK_HPP

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/blockchain/define.hpp>
#include <bitcoin/blockchain/interface/simple_chain.hpp>
#include <bitcoin/blockchain/settings.hpp>
#include <bitcoin/blockchain/validation/fork.hpp>
#include <bitcoin/blockchain/validation/populate_block.hpp>

namespace libbitcoin {
namespace blockchain {

/// This class is thread safe.
class BCB_API validate_block
{
public:
    typedef handle0 result_handler;

    validate_block(threadpool& pool, const simple_chain& chain,
        const settings& settings);

    void stop();

    code check(block_const_ptr block) const;

    void accept(fork::const_ptr fork, size_t index,
        result_handler handler) const;

    void connect(fork::const_ptr fork, size_t index,
        result_handler handler) const;

protected:
    bool stopped() const;

private:
    static code verify_script(const chain::transaction& tx,
        uint32_t input_index, uint32_t flags, bool use_libconsensus);
    static void report(block_const_ptr block, asio::time_point start_time,
        const std::string& token);

    void handle_accepted(const code& ec, block_const_ptr block,
        result_handler handler) const;
    void connect_input(const chain::transaction& tx,
        size_t input_index, uint32_t flags, result_handler handler) const;
    void handle_connect(const code& ec, block_const_ptr block,
        asio::time_point start_time, result_handler handler) const;

    // These are thread safe.
    std::atomic<bool> stopped_;
    const bool use_libconsensus_;
    populate_block populator_;
    mutable dispatcher dispatch_;
};

} // namespace blockchain
} // namespace libbitcoin

#endif
