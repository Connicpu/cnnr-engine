#pragma once

#include "VsCode.h"
#include <Common/String.h>
#include <Common/optional.h>
#include <connorlib/serialization/toml.h>
#include <connorlib/serialization/json.h>
#include <connorlib/serialization/conversion.h>
#include <concurrent_queue.h>
#include <atomic>
#include <thread>
#include <mutex>

namespace vsc
{
    class ProtocolMessage
    {
    public:
        int seq;
        String type;

        ProtocolMessage(String type)
            : seq(0), type(std::move(type).into_owned())
        {
        }

        ProtocolMessage(String type, int seq)
            : seq(seq), type(std::move(type).into_owned())
        {
        }

        TOML::Value Serialize() const
        {
            auto value = TOML::Value::Table();

            auto &table = value->GetTable();
            table.Insert("seq", TOML::Value::I64(seq));
            table.Insert("type", TOML::Value::String(type.copy_owned()));

            return std::move(value);
        }
    };

    class Request : public ProtocolMessage
    {
    public:
        String command;
        TOML::Value arguments;

        Request(int id, String cmd, TOML::Value args)
            : ProtocolMessage("request"_s, id), command(std::move(cmd)), arguments(std::move(args))
        {
        }

        static Request Deserialize(const TOML::Table &value)
        {
            return Request(value["seq"]->GetI64(), value["cmd"]->GetString(), value["arg"]->Clone());
        }
    };

    class ResponseBody
    {
    public:
        virtual TOML::Value Serialize() const = 0;
    };

    class Response : public ProtocolMessage
    {
    public:
        bool success;
        String message;
        int request_seq;
        String command;
        std::unique_ptr<ResponseBody> body;

        Response(const Request &req)
            : ProtocolMessage("response"_s),
              success(true), request_seq(req.seq), command(req.command)
        {
        }

        void SetBody(std::unique_ptr<ResponseBody> bdy)
        {
            success = true;
            body = std::move(bdy);
        }

        void SetErrorBody(String msg, std::unique_ptr<ResponseBody> bdy = nullptr)
        {
            success = false;
            message = std::move(msg);
            body = std::move(bdy);
        }

        TOML::Value Serialize() const
        {
            auto value = ProtocolMessage::Serialize();

            auto &table = value->GetTable();
            table.Insert("success", TOML::Value::Bool(success));
            table.Insert("message", TOML::Value::String(message.copy_owned()));
            table.Insert("request_seq", TOML::Value::I64(request_seq));
            table.Insert("command", TOML::Value::String(command.copy_owned()));
            if (body) table.Insert("body", body->Serialize());

            return std::move(value);
        }
    };

    class Event : public ProtocolMessage
    {
    public:
        String event;
        TOML::Value body;

        Event(String type, TOML::Value body)
            : ProtocolMessage("event"), event(std::move(type)), body(std::move(body))
        {
        }

        TOML::Value Serialize() const
        {
            auto value = ProtocolMessage::Serialize();

            auto &table = value->GetTable();
            table.Insert("message", TOML::Value::String(event.copy_owned()));
            table.Insert("body", body->Clone());

            return std::move(value);
        }
    };

    class ProtocolServer
    {
    public:
        ProtocolServer()
        {
            read_thread_ = std::thread([this]()
            {
                while (!request_stop_.load())
                {
                    try
                    {
                        read_queue_.push(VsCode::Recv()->ToToml());
                    }
                    catch (...)
                    {
                        break;
                    }
                }
            });
        }

        void Send(const TOML::ValueRef &data)
        {
            std::lock_guard<std::mutex> guard{ write_lock_ };

            VsCode::Send(*data.ToJson().Ref());
        }

        std::optional<TOML::Value> Recv()
        {
            TOML::Value value;
            if (read_queue_.try_pop(value))
                return std::move(value);
            return std::nullopt;
        }

        void Stop()
        {
            request_stop_ = true;
            fclose(stdin);
            read_thread_.join();
        }

    private:
        concurrency::concurrent_queue<TOML::Value> read_queue_;
        std::thread read_thread_;
        std::atomic<bool> request_stop_;
        std::mutex write_lock_;
    };
}

