#ifndef __COO_H__
#define __COO_H__

#include "../tensor/algstrct.h"

namespace CTF_int {

  class CSR_Matrix;
  class bivar_function;

  int64_t get_coo_size(int64_t nnz, int val_size);

  /** \brief serialized matrix in coordinate format, meaning three arrays of dimension nnz are stored, one of values, and two of row and column indices */
  class COO_Matrix{
    public:
      /** \brief serialized buffer containing info and data */
      char * all_data;
      
      /** 
       * \brief constructor that allocates empty buffer
       * \param[in] nnz number of nonzeros
       * \param[in] sr algebraic structure
       */
      COO_Matrix(int64_t nnz, algstrct const * sr);

      /** 
       * \brief constructor that acccepts data buffer
       * \param[in] all_data preallocated serialized COO Matrix buffer
       */
      COO_Matrix(char * all_data);

      /** 
       * \brief constructor that constructs serialized COO Matrix from a CSR_Matrix
       * \param[in] csr a matrix in CSR format
       * \param[in] sr algebraic structure
       */
      COO_Matrix(CSR_Matrix const & csr, algstrct const * sr);

      /** \brief retrieves number of nonzeros out of all_data */
      int64_t nnz() const;

      /** \brief retrieves buffer size out of all_data */
      int64_t size() const;

      /** \brief retrieves number of rows out of all_data */
      int nrow() const;
      
      /** \brief retrieves number of columns out of all_data */
      int ncol() const;
      
      /** \brief retrieves matrix entry size out of all_data */
      int val_size() const;

      /** \brief retrieves pointer to array of values out of all_data */
      char * vals() const;

      /** \brief retrieves pointer to array row indices of each value */
      int * rows() const;

      /** \brief retrieves pointer to array of column indices for each value */
      int * cols() const;

      /**
       * \brief folds tensor data into COO format based on prespecification of row and column modes
       * \param[in] nz number of nonzers
       * \param[in] order number of tensor modes
       * \param[in] lens ranges of tensor modes
       * \param[in] ordering reordering of tensor modes
       * \param[in] nrow_idx number of modes to fold into rows
       * \param[in] tsr_data in key-value pair format
       * \param[in] sr algebraic structure
       * \param[in] phase dimensions of the blocking grid
       */
      void set_data(int64_t nz, int order, int const * lens, int const * ordering, int nrow_idx, char const * tsr_data, algstrct const * sr, int const * phase);

      /**
       * \brief unfolds tensor data from COO format based on prespecification of row and column modes
       * \param[in] nz number of nonzers
       * \param[in] order number of tensor modes
       * \param[in] lens ranges of tensor modes
       * \param[in] rev_ordering reordering of tensor modes
       * \param[in] nrow_idx number of modes to fold into rows
       * \param[in,out] tsr_data in key-value pair format
       * \param[in] sr algebraic structure
       * \param[in] phase dimensions of the blocking grid
       * \param[in] phase_rank index of this block in grid
       */
      void get_data(int64_t nz, int order, int const * lens, int const * rev_ordering, int nrow_idx, char * tsr_data, algstrct const * sr, int const * phase, int const * phase_rank);

      /**
       * \brief computes C = beta*C + func(alpha*A*B) where A is a COO_Matrix, while B and C are dense
       */
      static void coomm(char const * A, algstrct const * sr_A, int m, int n, int k, char const * alpha, char const * B, algstrct const * sr_B, char const * beta, char * C, algstrct const * sr_C, bivar_function const * func);

  };
}

#endif
