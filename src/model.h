/*!
 * \file model.h
 * \brief define local model reader
 */

#ifndef LIGHTLDA_MODEL_H_
#define LIGHTLDA_MODEL_H_

#include <memory>
#include <string>

#include "common.h"
#include <multiverso/meta.h>

namespace multiverso 
{ 
    template<typename T> class Row;
    class Table;
     
namespace lightlda
{
    class Meta;
    class Trainer;
    class IDataStream;

    /*! \brief interface for acceess to model */
    class ModelBase
    {
    public:
        virtual ~ModelBase() {}
        virtual Row<int32_t>& GetWordTopicRow(integer_t word_id) = 0;
        virtual Row<int64_t>& GetSummaryRow() = 0;
        virtual void AddWordTopic(integer_t word_id, integer_t topic_id, 
            int32_t delta) = 0;
        virtual void AddSummary(integer_t topic_id, int64_t delta) = 0;
        
        virtual Row<int32_t>& GetTopicFrequencyRow(integer_t topic_id) = 0; 
        virtual Row<int32_t>& GetDocLengthRow() = 0; 
        virtual void AddTopicFrequency(integer_t topic_id, integer_t freq, 
            int32_t delta) = 0;
        virtual void AddDocLength(integer_t doc_len, int32_t delta) = 0;
    };

    /*! \brief model based on local buffer */
    class LocalModel : public ModelBase
    {
    public:
        explicit LocalModel(Meta * meta);
        void Init();

        Row<int32_t>& GetWordTopicRow(integer_t word_id) override;
        Row<int64_t>& GetSummaryRow() override;
        void AddWordTopic(integer_t word_id, integer_t topic_id, 
            int32_t delta) override;
        void AddSummary(integer_t topic_id, int64_t delta) override;

        Row<int32_t>& GetTopicFrequencyRow(integer_t topic_id) override; 
        Row<int32_t>& GetDocLengthRow() override; 
        void AddTopicFrequency(integer_t topic_id, integer_t freq, 
            int32_t delta) override;
        void AddDocLength(integer_t doc_len, int32_t delta) override;

    private:
        void CreateTable();
        void LoadTable();
        void LoadWordTopicTable(const std::string& model_fname);
        void LoadSummaryTable(const std::string& model_fname);

        std::unique_ptr<Table> word_topic_table_;
        std::unique_ptr<Table> summary_table_;
        std::unique_ptr<Table> topic_frequency_table_;
        std::unique_ptr<Table> doc_length_table_;
        Meta* meta_;

        LocalModel(const LocalModel&) = delete;
        void operator=(const LocalModel&) = delete;
    };

    /*! \brief model based on parameter server */
    class PSModel : public ModelBase
    {
    public:
        static void Init(Meta* meta, IDataStream * data_stream);
    private:
        static void CreateTable();
        static void ConfigTable(Meta* meta);
        static void LoadTable(Meta* meta, IDataStream * data_stream);

    public:
        explicit PSModel(Trainer* trainer) : trainer_(trainer) {}

        Row<int32_t>& GetWordTopicRow(integer_t word_id) override;
        Row<int64_t>& GetSummaryRow() override;
        void AddWordTopic(integer_t word_id, integer_t topic_id, 
            int32_t delta) override;
        void AddSummary(integer_t topic_id, int64_t delta) override;

        Row<int32_t>& GetTopicFrequencyRow(integer_t topic_id) override; 
        Row<int32_t>& GetDocLengthRow() override; 
        void AddTopicFrequency(integer_t topic_id, integer_t freq, 
            int32_t delta) override;
        void AddDocLength(integer_t doc_len, int32_t delta) override;

    private:
        Trainer* trainer_;

        PSModel(const PSModel&) = delete;
        void operator=(const PSModel&) = delete;
    };

} // namespace lightlda
} // namespace multiverso

#endif // LIGHTLDA_MODEL_H_
