/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 *
 * Description:
 * TODO: add description
 */

#include "SeedKey.hxx"

#include <diagnosis-tester/DiagnosisTesterManagerInterface.hxx>

#include <spdlog/sinks/stdout_color_sinks.h>

#include <thread>

#include <iostream>

bool unlockSecurityLevel(std::shared_ptr<spdlog::logger>& logger, std::shared_ptr<diagnosisTester::DiagnosisTesterInterface>& tester, int seedId, int keyId)
{
    std::vector<diagnosisTester::DiagnosisTesterInterface::NextGenResponseData> result;
    diagnosisTester::DiagnosisTesterInterface::NextGenRv                        rv;

    logger->info("requestSecuritySeedNg");
    rv = tester->requestSecuritySeedNg(diagnosisTester::DiagnosisTesterInterface::UdsAddressing::Physical, seedId, result);

    std::vector<std::uint8_t> key;
    if (!calculateSecurityPassword(result[0].payload, seedId, key))
    {
        return false;
    }

    logger->info("writeSecurityKey");
    const int rvKey = tester->writeSecurityKey(keyId, key);

    rv = tester->requestSecuritySeedNg(diagnosisTester::DiagnosisTesterInterface::UdsAddressing::Physical, seedId, result);

    return true;
}

void exec(std::shared_ptr<spdlog::logger>& logger, std::shared_ptr<diagnosisTester::DiagnosisTesterInterface>& tester)
{
    tester->testerPresentCyclicEnable(std::chrono::milliseconds(4000));

    std::vector<diagnosisTester::DiagnosisTesterInterface::NextGenResponseData> result;
    diagnosisTester::DiagnosisTesterInterface::NextGenRv                        rv;

    logger->info("connect");
    tester->connect(0x42);

    logger->info("diagnosticSessionControlNg");
    rv = tester->diagnosticSessionControlNg(diagnosisTester::DiagnosisTesterInterface::UdsAddressing::Physical,
                                            diagnosisTester::DiagnosisTesterInterface::DiagnosticSessions::Extended,
                                            result);
    if (rv != diagnosisTester::DiagnosisTesterInterface::NextGenRv::Ok)
    {
        return;
    }

    if (result.size() != 1)
    {
        return;
    }

    diagnosisTester::DiagnosisTesterInterface::NextGenResponseData& resp = result.at(0);

    if (resp.nrc != 0)
    {
        return;
    }

    logger->info("unlockSecurityLevel");
    if (!unlockSecurityLevel(logger, tester, 1, 2))
    {
        return;
    }

    logger->info("ecuResetNg");
    rv = tester->ecuResetNg(diagnosisTester::DiagnosisTesterInterface::UdsAddressing::Physical,
                            diagnosisTester::DiagnosisTesterInterface::ResetTypes::Hard,
                            result);
    if (rv != diagnosisTester::DiagnosisTesterInterface::NextGenRv::Ok)
    {
        return;
    }

    if (result.size() != 1)
    {
        return;
    }

    resp = result.at(0);

    if (resp.nrc != 0)
    {
        return;
    }

    logger->info("sleep");
    std::this_thread::sleep_for(std::chrono::seconds(5));

    logger->info("testerPresentNg");
    rv = tester->testerPresentNg(diagnosisTester::DiagnosisTesterInterface::UdsAddressing::Physical, false, result);
    if (rv != diagnosisTester::DiagnosisTesterInterface::NextGenRv::Ok)
    {
        return;
    }

    if (result.size() != 1)
    {
        return;
    }

    resp = result.at(0);

    if (resp.nrc != 0)
    {
        return;
    }

    logger->info("diagnosticSessionControlNg");
    rv = tester->diagnosticSessionControlNg(diagnosisTester::DiagnosisTesterInterface::UdsAddressing::Physical,
                                            diagnosisTester::DiagnosisTesterInterface::DiagnosticSessions::Extended,
                                            result);
    if (rv != diagnosisTester::DiagnosisTesterInterface::NextGenRv::Ok)
    {
        return;
    }

    if (result.size() != 1)
    {
        return;
    }

    resp = result.at(0);

    if (resp.nrc != 0)
    {
        return;
    }

    logger->info("routineControlStartNg");
    rv = tester->routineControlStartNg(diagnosisTester::DiagnosisTesterInterface::UdsAddressing::Physical, 0x0203, {}, result);

    if (rv != diagnosisTester::DiagnosisTesterInterface::NextGenRv::Ok)
    {
        return;
    }

    if (result.size() != 1)
    {
        return;
    }

    resp = result.at(0);

    if (resp.nrc != 0)
    {
        return;
    }

    logger->info("unlockSecurityLevel");
    if (!unlockSecurityLevel(logger, tester, 1, 2))
    {
        return;
    }

    logger->info("controlDtcSettingNg");
    tester->controlDtcSettingNg(diagnosisTester::DiagnosisTesterInterface::UdsAddressing::Physical,
                                diagnosisTester::DiagnosisTesterInterface::DtcSettingControlSubfunction::Off,
                                {0xFF, 0xFF, 0xFF},
                                result);

    if (rv != diagnosisTester::DiagnosisTesterInterface::NextGenRv::Ok)
    {
        return;
    }

    if (result.size() != 1)
    {
        return;
    }

    resp = result.at(0);

    if (resp.nrc != 0)
    {
        return;
    }

    logger->info("controlDtcSettingNg");
    tester->communicationControlNg(diagnosisTester::DiagnosisTesterInterface::UdsAddressing::Physical,
                                   diagnosisTester::DiagnosisTesterInterface::CommunicationControlSubfunction::EnableRxAndDisableTx,
                                   0x01,
                                result);

    if (rv != diagnosisTester::DiagnosisTesterInterface::NextGenRv::Ok)
    {
        return;
    }

    if (result.size() != 1)
    {
        return;
    }

    resp = result.at(0);

    if (resp.nrc != 0)
    {
        return;
    }

    logger->info("diagnosticSessionControlNg");
    rv = tester->diagnosticSessionControlNg(diagnosisTester::DiagnosisTesterInterface::UdsAddressing::Physical,
                                            diagnosisTester::DiagnosisTesterInterface::DiagnosticSessions::Programming,
                                            result);
    if (rv != diagnosisTester::DiagnosisTesterInterface::NextGenRv::Ok)
    {
        return;
    }

    if (result.size() != 1)
    {
        return;
    }

    resp = result.at(0);

    if (resp.nrc != 0)
    {
        return;
    }

    logger->info("sleep");
    std::this_thread::sleep_for(std::chrono::seconds(3));

    logger->info("unlockSecurityLevel");
    if (!unlockSecurityLevel(logger, tester, 0x11, 0x12))
    {
        return;
    }

    logger->info("done");

}

int main(int argc, char* argv[])
{
    auto logger = spdlog::stdout_color_mt("flashcli");
    logger->set_level(spdlog::level::trace);

    auto& dtm    = diagnosisTester::DiagnosisTesterManagerInterface::getInstance();
    auto  tester = dtm.getTester(logger, "can0", 0x680, 0x00, 0xFE, 0x680);

    if (!tester)
    {
        return EXIT_FAILURE;
    }

    const int udsTimeoutFactor = 1;
    tester->setIsoTpParameter(diagnosisTester::Iso15765Parameter::StMinTx, 20);
    tester->setIsoTpParameter(diagnosisTester::Iso15765Parameter::StMin, 20);
    tester->setIsoTpParameter(diagnosisTester::Iso15765Parameter::Blocksize, 20);
    tester->setIsoTpParameter(diagnosisTester::Iso15765Parameter::PaddingValue, 0xAA);
    tester->setIsoTpParameter(diagnosisTester::Iso15765Parameter::PaddingStatus, 1);
    tester->setIsoTpParameter(diagnosisTester::Iso15765Parameter::TimeoutNar, 70);
    tester->setIsoTpParameter(diagnosisTester::Iso15765Parameter::TimeoutNcr, 150);
    tester->setIsoTpParameter(diagnosisTester::Iso15765Parameter::TimeoutNas, 70);
    tester->setIsoTpParameter(diagnosisTester::Iso15765Parameter::TimeoutNbs, 150);
    tester->setIsoTpParameter(diagnosisTester::Iso15765Parameter::NWftMax, 20);
    tester->setCommunicationParameter(diagnosisTester::DiagnosisTesterInterface::CommunicationParameter::UdsP2Timeout, 1500 * udsTimeoutFactor);
    tester->setCommunicationParameter(diagnosisTester::DiagnosisTesterInterface::CommunicationParameter::UdsP2ExtTimeout, 5000);
    tester->setCommunicationParameter(diagnosisTester::DiagnosisTesterInterface::CommunicationParameter::UdsFinalResponseTimeout, 30000 + 3000);

    exec(logger, tester);

    return 0;
}