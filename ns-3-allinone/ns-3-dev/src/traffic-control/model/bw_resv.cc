/*
 * bw_resv.cc
 *
 *  Created on: Jan 10, 2019
 *      Author: vamsi
 */

#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/net-device-queue-interface.h"
#include "bw_resv.h"
#include "queue-disc.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("BwResvQueueDisc");

NS_OBJECT_ENSURE_REGISTERED (BwResvQueueDisc);

TypeId BwResvQueueDisc::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BwResvQueueDisc")
    .SetParent<QueueDisc> ()
    .SetGroupName ("TrafficControl")
    .AddConstructor<BwResvQueueDisc> ()
    .AddAttribute ("MaxSize",
                   "The max queue size",
                   QueueSizeValue (QueueSize ("1000p")),
                   MakeQueueSizeAccessor (&QueueDisc::SetMaxSize,
                                          &QueueDisc::GetMaxSize),
                   MakeQueueSizeChecker ())
    .AddAttribute ("SizeDetnet",
                   "Size of the first bucket in bytes",
                   UintegerValue (0),
                   MakeUintegerAccessor (&BwResvQueueDisc::SetSizeDetnet),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("SizeOther",
                   "Size of the second bucket in bytes. If null, it is initialized"
                   " to the MTU of the receiving NetDevice (if any)",
                   UintegerValue (0),
                   MakeUintegerAccessor (&BwResvQueueDisc::SetSizeOther),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("RateDetnet",
                   "Rate at which tokens enter the first bucket in bps or Bps.",
                   DataRateValue (DataRate ("1Kb/s")),
                   MakeDataRateAccessor (&BwResvQueueDisc::SetRateDetnet),
                   MakeDataRateChecker ())
    .AddAttribute ("RateOther",
                   "Rate at which tokens enter the second bucket in bps or Bps."
                   "If null, there is no second bucket",
                   DataRateValue (DataRate ("1Kb/s")),
                   MakeDataRateAccessor (&BwResvQueueDisc::SetRateOther),
                   MakeDataRateChecker ())
    .AddTraceSource ("TokensInFirstBucket",
                     "Number of First Bucket Tokens in bytes",
                     MakeTraceSourceAccessor (&BwResvQueueDisc::m_detnet_tokens),
                     "ns3::TracedValueCallback::Uint32")
    .AddTraceSource ("TokensInSecondBucket",
                     "Number of Second Bucket Tokens in bytes",
                     MakeTraceSourceAccessor (&BwResvQueueDisc::m_other_tokens),
                     "ns3::TracedValueCallback::Uint32")
  ;

  return tid;
}

BwResvQueueDisc::BwResvQueueDisc ()
  : QueueDisc (QueueDiscSizePolicy::SINGLE_CHILD_QUEUE_DISC)
{
  NS_LOG_FUNCTION (this);
}

BwResvQueueDisc::~BwResvQueueDisc ()
{
  NS_LOG_FUNCTION (this);
}

void
BwResvQueueDisc::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  QueueDisc::DoDispose ();
}

void
BwResvQueueDisc::SetSizeDetnet (uint32_t sizedetnet)
{
  NS_LOG_FUNCTION (this << sizedetnet);
  m_sizedetnet = sizedetnet;
}

uint32_t
BwResvQueueDisc::GetSizeDetnet (void) const
{
  NS_LOG_FUNCTION (this);
  return m_sizedetnet;
}

void
BwResvQueueDisc::SetSizeOther (uint32_t sizeother)
{
  NS_LOG_FUNCTION (this << sizeother);
  m_sizeother = sizeother;
}

uint32_t
BwResvQueueDisc::GetSizeOther (void) const
{
  NS_LOG_FUNCTION (this);
  return m_sizeother;
}

void
BwResvQueueDisc::SetRateDetnet (DataRate ratedetnet)
{
  NS_LOG_FUNCTION (this << ratedetnet);
  m_ratedetnet = ratedetnet;
}

DataRate
BwResvQueueDisc::GetRateDetnet (void) const
{
  NS_LOG_FUNCTION (this);
  return m_ratedetnet;
}

void
BwResvQueueDisc::SetRateOther (DataRate rateother)
{
  NS_LOG_FUNCTION (this << rateother);
  m_rateother = rateother;
}

DataRate
BwResvQueueDisc::GetRateOther (void) const
{
  NS_LOG_FUNCTION (this);
  return m_rateother;
}

uint32_t
BwResvQueueDisc::GetTokensDetnet (void) const
{
  NS_LOG_FUNCTION (this);
  return m_detnet_tokens;
}

uint32_t
BwResvQueueDisc::GetTokensOther (void) const
{
  NS_LOG_FUNCTION (this);
  return m_other_tokens;
}

bool
BwResvQueueDisc::DoEnqueue (Ptr<QueueDiscItem> item)
{
  NS_LOG_FUNCTION (this << item);

  //int32_t ret = 1; /*Classify (item);*/
  int32_t band = 0;
  Time now = Simulator::Now ();
  double delta = (now  - m_timeCheckPoint).GetSeconds ();
  m_timeCheckPoint=now;

  uint32_t h = item->Hash ();
  //printf("%u\n",h);

/*  if (ret == PacketFilter::PF_NO_MATCH)
    {
      NS_LOG_DEBUG ("No filter has been able to classify this packet, using bwresv.");
      band = 1;
    }
  else
	  band = ret;*/
  if(h==608124638)band=1;
  else
	  band=0;

//  //Strict reservation
//  if(band==0){
//	  double delta = (now  - m_timeCheckPoint_detnet).GetSeconds ();
//	  m_timeCheckPoint_detnet=now;
//  uint32_t credit = (m_ratedetnet.GetBitRate ()*delta);
//  if(m_detnet_tokens > credit)m_detnet_tokens -= (m_ratedetnet.GetBitRate ()*delta);
//  else m_detnet_tokens=0;
//  }
//  else if(band==1){
//	  double delta = (now  - m_timeCheckPoint_other).GetSeconds ();
//	    m_timeCheckPoint_other=now;
//  uint32_t credit=(m_rateother.GetBitRate ()*delta);
//  if(m_other_tokens > credit)m_other_tokens -= (m_rateother.GetBitRate ()*delta);
//  else m_other_tokens=0;
//  }
//  ////////////////////
//
  //Overall bandwidth usage priority
  uint32_t credit = (m_ratedetnet.GetBitRate ()*delta);
  uint32_t old_nbl=nbl+1;
  while(old_nbl>nbl && nbl>0){
	  old_nbl=nbl;
	  if(m_detnet_tokens>credit){
		  m_detnet_tokens -= credit;
		  credit=0;
	  }
	  else{
		  if(!m_detnet_tokens){
			  credit-=m_detnet_tokens;
			  m_detnet_tokens=0;
			  nbl--;
		  }
		  if (m_other_tokens>credit){
			  m_other_tokens -=credit;
			  credit=0;
		  }
		  else{
			  credit -=m_other_tokens;
			  m_other_tokens=0;
			  nbl--;

		  }
	  }
  }

  uint32_t pktsize = (item->GetSize ())*8;
 /* if(h==1824837955 || h==3351919300){ //870704681
	  DropBeforeEnqueue (item, FORCED_DROP);
	  return false;
  }*/ // Test for hash values

  if(band==0 && (m_detnet_tokens+pktsize <= threshold)){
	  if(!m_detnet_tokens)nbl++;
	  m_detnet_tokens += pktsize;
	  //printf("%u\n",nbl);
  }
  else if (band==0 && (m_detnet_tokens+pktsize > threshold)){
	  DropBeforeEnqueue (item, FORCED_DROP);
	  return false;
  }
  else if (band==1 && (m_other_tokens +pktsize <= threshold)){
	  if(!m_other_tokens)nbl++;
	  m_other_tokens += pktsize;
	  //printf("%u\n",nbl);
  }
  else if(band==1 && (m_other_tokens +pktsize > threshold)){
	  DropBeforeEnqueue (item, FORCED_DROP);
	  return false;
  }

  bool retval = GetQueueDiscClass (0)->GetQueueDisc ()->Enqueue (item);

  // If Queue::Enqueue fails, QueueDisc::Drop is called by the child queue
  // disc because QueueDisc::AddQueueDiscClass sets the drop callback

  NS_LOG_LOGIC ("Current queue size: " << GetNPackets () << " packets, " << GetNBytes () << " bytes");

  return retval;
}

Ptr<QueueDiscItem>
BwResvQueueDisc::DoDequeue (void)
{
  NS_LOG_FUNCTION (this);
  //Ptr<const QueueDiscItem> itemPeek = GetQueueDiscClass (0)->GetQueueDisc ()->Peek ();

//  if (GetQueueDiscClass (0)->GetQueueDisc ()-> IsEmpty () )
//	  return 0;
//  else{

  	  Ptr<QueueDiscItem> item = GetQueueDiscClass (0)->GetQueueDisc ()->Dequeue ();
	  //if(item!= NULL )
	  return item;
	  //else return 0;
//  }
/*
  if (itemPeek)
    {
      uint32_t pktSize = itemPeek->GetSize ();
      NS_LOG_LOGIC ("Next packet size " << pktSize);

      int64_t btoks = 0;
      int64_t ptoks = 0;
      Time now = Simulator::Now ();

      double delta = (now  - m_timeCheckPoint).GetSeconds ();
      NS_LOG_LOGIC ("Time Difference delta " << delta);

      if (m_peakRate > DataRate ("0bps"))
        {
          ptoks =  m_ptokens + round (delta * (m_rateother.GetBitRate () / 8));
          if (ptoks > m_mtu)
            {
              ptoks = m_mtu;
            }
          NS_LOG_LOGIC ("Number of ptokens we can consume " << ptoks);
          NS_LOG_LOGIC ("Required to dequeue next packet " << pktSize);
          ptoks -= pktSize;
        }

      btoks = m_btokens + round (delta * (m_rate.GetBitRate () / 8));

      if (btoks > m_burst)
        {
          btoks = m_burst;
        }

      NS_LOG_LOGIC ("Number of btokens we can consume " << btoks);
      NS_LOG_LOGIC ("Required to dequeue next packet " << pktSize);
      btoks -= pktSize;

      if ((btoks|ptoks) >= 0) // else packet blocked
        {
          Ptr<QueueDiscItem> item = GetQueueDiscClass (0)->GetQueueDisc ()->Dequeue ();
          if (!item)
            {
              NS_LOG_DEBUG ("That's odd! Expecting the peeked packet, we got no packet.");
              return item;
            }

          m_timeCheckPoint = now;
          m_btokens = btoks;
          m_ptokens = ptoks;

          NS_LOG_LOGIC (m_btokens << " btokens and " << m_ptokens << " ptokens after packet dequeue");
          NS_LOG_LOGIC ("Current queue size: " << GetNPackets () << " packets, " << GetNBytes () << " bytes");

          return item;
        }

      // the watchdog timer setup.
      // A packet gets blocked if the above if condition is not satisfied, i.e.
      //both the ptoks and btoks are less than zero. In that case we have to
      //schedule the waking of queue when enough tokens are available.
      if (m_id.IsExpired () == true)
        {
          Time requiredDelayTime = std::max (m_rate.CalculateBytesTxTime (-btoks),
                                             m_peakRate.CalculateBytesTxTime (-ptoks));

          m_id = Simulator::Schedule (requiredDelayTime, &QueueDisc::Run, this);
          NS_LOG_LOGIC("Waking Event Scheduled in " << requiredDelayTime);
        }
    }
  */

  //return 0;
}

bool
BwResvQueueDisc::CheckConfig (void)
{
  NS_LOG_FUNCTION (this);
  if (GetNInternalQueues () > 0)
    {
      NS_LOG_ERROR ("TbfQueueDisc cannot have internal queues");
      return false;
    }

  if (GetNPacketFilters () > 0)
    {
      NS_LOG_ERROR ("TbfQueueDisc cannot have packet filters");
      return false;
    }

  if (GetNQueueDiscClasses () == 0)
    {
      // create a FIFO queue disc
      ObjectFactory factory;
      factory.SetTypeId ("ns3::FifoQueueDisc");
      Ptr<QueueDisc> qd = factory.Create<QueueDisc> ();

      if (!qd->SetMaxSize (GetMaxSize ()))
        {
          NS_LOG_ERROR ("Cannot set the max size of the child queue disc to that of TbfQueueDisc");
          return false;
        }
      qd->Initialize ();

      Ptr<QueueDiscClass> c = CreateObject<QueueDiscClass> ();
      c->SetQueueDisc (qd);
      AddQueueDiscClass (c);
    }

  if (GetNQueueDiscClasses () != 1)
    {
      NS_LOG_ERROR ("TbfQueueDisc needs 1 child queue disc");
      return false;
    }
/*
  if (m_sizeother == 0)
    {
      Ptr<NetDeviceQueueInterface> ndqi = GetNetDeviceQueueInterface ();
      Ptr<NetDevice> dev;
      // if the NetDeviceQueueInterface object is aggregated to a
      // NetDevice, get the MTU of such NetDevice
      if (ndqi && (dev = ndqi->GetObject<NetDevice> ()))
        {
          m_sizeother = dev->GetMtu ();
        }
    }

  if (m_sizeother == 0 && m_rateother > DataRate ("0bps"))
    {
      NS_LOG_ERROR ("A non-null peak rate has been set, but the mtu is null. No packet will be dequeued");
      return false;
    }

  if (m_burst <= m_mtu)
    {
      NS_LOG_WARN ("The size of the first bucket (" << m_burst << ") should be "
                    << "greater than the size of the second bucket (" << m_mtu << ").");
    }

  if (m_peakRate > DataRate ("0bps") && m_peakRate <= m_rate)
    {
      NS_LOG_WARN ("The rate for the second bucket (" << m_peakRate << ") should be "
                    << "greater than the rate for the first bucket (" << m_rate << ").");
    }
*/
  return true;
}

void
BwResvQueueDisc::InitializeParams (void)
{
  NS_LOG_FUNCTION (this);
  // Token Buckets are full at the beginning.
  m_detnet_tokens = m_sizedetnet;
  m_other_tokens = m_sizeother;
  // Initialising other variables to 0.
  m_timeCheckPoint_detnet = Seconds (0);
  m_timeCheckPoint_other = Seconds (0);
  m_timeCheckPoint = Seconds (0);
  m_id = EventId ();
  threshold=1500*50;
  nbl=0;
}

} // namespace ns3


